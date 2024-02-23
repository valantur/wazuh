'''
copyright: Copyright (C) 2015-2024 Wazuh Inc.

           Created by Wazuh, Inc. <info@wazuh.com>.

           This program is free software; you can redistribute it and/or modify it under the terms of GPLv2

type: integration

brief: The 'wazuh-logcollector' daemon monitors configured files and commands for new log messages.
       Specifically, these tests will check if the logcollector detects invalid values for the
       'reconnect_time' tag. Log data collection is the real-time process of making sense out
       of the records generated by servers or devices. This component can receive logs through
       text files or Windows event logs. It can also directly receive logs via remote syslog
       which is useful for firewalls and other such devices.

components:
    - logcollector

suite: configuration

targets:
    - agent

daemons:
    - wazuh-logcollector

os_platform:
    - windows

os_version:
    - Windows 10
    - Windows 8
    - Windows 7
    - Windows Server 2019
    - Windows Server 2016
    - Windows Server 2012
    - Windows Server 2003
    - Windows XP

references:
    - https://documentation.wazuh.com/current/user-manual/capabilities/log-data-collection/index.html
    - https://documentation.wazuh.com/current/user-manual/reference/ossec-conf/localfile.html#reconnect-time

tags:
    - logcollector_configuration
'''
import pytest

from pathlib import Path

from wazuh_testing.constants.paths.logs import WAZUH_LOG_PATH
from wazuh_testing.constants.daemons import LOGCOLLECTOR_DAEMON
from wazuh_testing.modules.logcollector import patterns
from wazuh_testing.tools.monitors import file_monitor
from wazuh_testing.utils import callbacks, configuration
from wazuh_testing.utils.services import control_service
from wazuh_testing.utils.file import truncate_file

from . import TEST_CASES_PATH, CONFIGURATIONS_PATH


LOG_COLLECTOR_GLOBAL_TIMEOUT = 40


# Marks
pytestmark = [pytest.mark.agent, pytest.mark.win32, pytest.mark.tier(level=0)]

# Test metadata, configuration and ids.
cases_path = Path(TEST_CASES_PATH, 'cases_basic_configuration_reconnect_time.yaml')
config_path = Path(CONFIGURATIONS_PATH, 'wazuh_basic_configuration_reconnect_time.yaml')
test_configuration, test_metadata, test_cases_ids = configuration.get_test_cases_data(cases_path)
test_configuration = configuration.load_configuration_template(config_path, test_configuration, test_metadata)

problematic_values = ['44sTesting', '9hTesting', '400mTesting', '3992']

# Test daemons to restart.
daemons_handler_configuration = {'all_daemons': True}


# Test function.
@pytest.mark.parametrize('test_configuration, test_metadata', zip(test_configuration, test_metadata), ids=test_cases_ids)
def test_configuration_reconnect_time(test_configuration, test_metadata, truncate_monitored_files,
                                      set_wazuh_configuration, daemons_handler_module, stop_logcollector):
    '''
    description: Check if the 'wazuh-logcollector' daemon detects invalid settings for the 'reconnect_time' tag.
                 For this purpose, the test will set a 'localfile' section using both valid and invalid values
                 for that tag. Finally, the test will verify that the 'analyzing' event is triggered when using
                 a valid value or if the 'invalid' event is generated when using an invalid one.

    wazuh_min_version: 4.2.0

    tier: 0

    parameters:
        - test_configuration:
            type: data
            brief: Configuration used in the test.
        - test_metadata:
            type: data
            brief: Configuration cases.
        - truncate_monitored_files:
            type: fixture
            brief: Reset the 'ossec.log' file and start a new monitor.
        - set_wazuh_configuration:
            type: fixture
            brief: Configure a custom environment for testing.
        - daemons_handler_module:
            type: fixture
            brief: Handler of Wazuh daemons.
        - stop_logcollector:
            type: fixture
            brief: Stop logcollector daemon.

    assertions:
        - Verify that the logcollector generates 'invalid' events when using invalid values
          for the 'reconnect_time' tag.
        - Verify that the logcollector monitors a log file when using valid values for the 'reconnect_time' tag.

    input_description: A configuration template (test_basic_configuration_reconnect_time) is contained in an
                       external YAML file (wazuh_basic_configuration.yaml). That template is combined with
                       different test cases defined in the module. Those include configuration settings
                       for the 'wazuh-logcollector' daemon.

    expected_output:
        - r'Analyzing event log.*'
        - r'Invalid reconnection time value. Changed to .* seconds.'

    tags:
        - invalid_settings
        - logs
    '''

    wazuh_log_monitor = file_monitor.FileMonitor(WAZUH_LOG_PATH)

    if test_metadata['valid_value']:
        control_service('start', daemon=LOGCOLLECTOR_DAEMON)
        callback = callbacks.generate_callback(patterns.LOGCOLLECTOR_ANALYZING_EVENT_LOG,
                                                            {'event_location': test_metadata['location']})
        wazuh_log_monitor.start(timeout=LOG_COLLECTOR_GLOBAL_TIMEOUT, callback=callback)
        assert (wazuh_log_monitor.callback_result != None), patterns.ERROR_GENERIC_MESSAGE
    else:
        if test_metadata['reconnect_time'] in problematic_values:
            pytest.xfail("Logcolector accepts invalid values. Issue: https://github.com/wazuh/wazuh/issues/8158")
        else:
            control_service('start', daemon=LOGCOLLECTOR_DAEMON)
            callback = callbacks.generate_callback(patterns.LOGCOLLECTOR_INVALID_RECONNECTION_TIME_VALUE,
                                                            {'severity': 'WARNING',
                                                             'default_value':'5'})
            wazuh_log_monitor.start(timeout=5, callback=callback)
            assert (wazuh_log_monitor.callback_result != None), patterns.ERROR_INVALID_RECONNECTION_TIME