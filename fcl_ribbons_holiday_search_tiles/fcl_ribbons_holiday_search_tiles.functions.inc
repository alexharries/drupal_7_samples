<?php

/**
 * @file
 * Functions which aren't Drupal hooks live in this file.
 */

/**
 * Constant definitions.
 */

// Whether caching is enabled by default or not.
const FCL_RIBBONS_HOLIDAY_SEARCH_TILES_CACHE_ENABLED_DEFAULT = TRUE;

// Cache expiry default time; 15 minutes, in seconds.
const FCL_RIBBONS_HOLIDAY_SEARCH_TILES_CACHE_EXPIRY_DEFAULT = 60 * 15;

// This flag indicates that a paragraph's maximum results should come from the
// module's defaults.
const FCL_RIBBONS_HOLIDAY_SEARCH_TILES_SEARCH_RESULTS_DEFAULT = -1;

// Default maximum number of results.
const FCL_RIBBONS_HOLIDAY_SEARCH_TILES_SEARCH_RESULTS_DEFAULT_LIMIT = 6;

// Default text to show in the "Show more results" link.
const FCL_RIBBONS_HOLIDAY_SEARCH_TILES_SHOW_MORE_RESULTS_DEFAULT_TEXT = 'Show more results';

// The variable name to store whether the cache is enabled.
const FCL_RIBBONS_HOLIDAY_SEARCH_TILES_CACHE_ENABLED_VARIABLE = 'fcl_ribbons_holiday_search_tiles_cache_enabled';
