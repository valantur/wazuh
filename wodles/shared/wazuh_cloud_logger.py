#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
#
# Copyright (C) 2015, Wazuh Inc.
# Created by Wazuh, Inc. <info@wazuh.com>.
# This program is free software; you can redistribute
# it and/or modify it under the terms of GPLv2

"""This module implements the strategy pattern to provide a flexible logging solution for Wazuh Cloud Modules"""

from abc import ABC, abstractmethod
import logging
import sys

################################################################################
# Classes
################################################################################


class WazuhLogStrategy(ABC):
    """
    LogStrategy interface.

    Defines the methods that need to be implemented by the concrete classes
    (GCP, AWS, and Azure) for logging.
    """

    @abstractmethod
    def info(self, message: str):
        """
        Log an INFO level message.

        Parameters
        ----------
        message : str
            The message to be logged.
        """
        pass

    @abstractmethod
    def debug(self, message: str):
        """
        Log a DEBUG level message.

        Parameters
        ----------
        message : str
            The message to be logged.
        """
        pass

    @abstractmethod
    def warning(self, message: str):
        """
        Log a WARNING level message.

        Parameters
        ----------
        message : str
            The message to be logged.
        """
        pass

    @abstractmethod
    def error(self, message: str):
        """
        Log an ERROR level message.

        Parameters
        ----------
        message : str
            The message to be logged.
        """
        pass

    @abstractmethod
    def critical(self, message: str):
        """
        Log a CRITICAL level message.

        Parameters
        ----------
        message : str
            The message to be logged.
        """
        pass


class WazuhCloudLogger:
    """
    WazuhCloudLogger class.

    Provides a flexible logging solution for Wazuh that supports
    GCP, AWS, and Azure integrations using the strategy pattern.
    """

    def __init__(self, strategy: WazuhLogStrategy, log_level: int) -> logging.Logger:
        """
        Initialize the Wazuh Cloud Logger class.

        Parameters
        ----------
        strategy : LogStrategy
            The logging strategy to be used (GCP, AWS, or Azure).
        log_level : int, optional
            The logging level (0 for WARNING, 1 for INFO, 2 for DEBUG), default is 1.
        """
        self.strategy = strategy
        self.logger = self.setup_logger(log_level)

    def setup_logger(self, log_level: int) -> logging.Logger:
        """
        Set up the logger.

        Parameters
        ----------
        log_level : int
            The logging level (0 for WARNING, 1 for INFO, 2 for DEBUG).

        Returns
        -------
        logging.Logger
            Configured logger instance.
        """
        logger = self.strategy.logger
        logger_level = logging.DEBUG if log_level == 2 else logging.INFO
        logger.setLevel(logger_level)
        handler = self._setup_handler()
        logger.addHandler(handler)
        return logger

    def _setup_handler(self) -> logging.Handler:
        """
        Set up the handler for the logger.

        Returns
        -------
        logging.Handler
            Configured handler instance.
        """
        handler = logging.StreamHandler(sys.stdout)
        handler.setFormatter(logging.Formatter('%(name)s - %(levelname)s - %(message)s'))
        return handler

    def info(self, message: str):
        """
        Log an INFO level message using the selected strategy.

        Parameters
        ----------
        message : str
            The message to be logged.
        """
        self.strategy.info(message)

    def debug(self, message: str):
        """
        Log a DEBUG level message using the selected strategy.

        Parameters
        ----------
        message : str
            The message to be logged.
        """
        self.strategy.debug(message)

    def warning(self, message: str):
        """
        Log a WARNING level message using the selected strategy.

        Parameters
        ----------
        message : str
            The message to be logged.
        """
        self.strategy.warning(message)

    def error(self, message: str):
        """
        Log an ERROR level message using the selected strategy.

        Parameters
        ----------
        message : str
            The message to be logged.
        """
        self.strategy.error(message)

    def critical(self, message: str):
        """
        Log a CRITICAL level message using the selected strategy.

        Parameters
        ----------
        message : str
            The message to be logged.
        """
        self.strategy.critical(message)
