/*
 * Wazuh content manager - Unit Tests
 * Copyright (C) 2015, Wazuh Inc.
 * November 03, 2023.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#ifndef _ZIP_DECOMPRESSOR_TEST_HPP
#define _ZIP_DECOMPRESSOR_TEST_HPP

#include "updaterContext.hpp"
#include "gtest/gtest.h"
#include <atomic>
#include <filesystem>
#include <memory>

// Output directories.
const auto OUTPUT_FOLDER {std::filesystem::temp_directory_path() / "zipDecompressor"};
const auto EXPECTED_CONTENTS_FOLDER {OUTPUT_FOLDER / CONTENTS_FOLDER};

/**
 * @brief Runs unit tests for ZipDecompressor
 *
 */
class ZipDecompressorTest : public ::testing::Test
{
protected:
    ZipDecompressorTest() = default;
    ~ZipDecompressorTest() override = default;

    std::shared_ptr<UpdaterContext> m_spContext; ///< Context used on tests.
    const std::atomic<bool> m_shouldRun {true};  ///< Interruption flag.

    /**
     * @brief Setup routine for each test fixture. Context initialization and output directories creation.
     *
     */
    void SetUp() override
    {
        m_spContext = std::make_shared<UpdaterContext>();
        m_spContext->spUpdaterBaseContext = std::make_shared<UpdaterBaseContext>(m_shouldRun);
        m_spContext->spUpdaterBaseContext->outputFolder = OUTPUT_FOLDER;

        std::filesystem::create_directory(OUTPUT_FOLDER);
        std::filesystem::create_directory(EXPECTED_CONTENTS_FOLDER);
    }

    /**
     * @brief Teardown routine for each test fixture. Output files removal.
     *
     */
    void TearDown() override
    {
        std::filesystem::remove_all(OUTPUT_FOLDER);
    }
};

#endif //_ZIP_DECOMPRESSOR_TEST_HPP