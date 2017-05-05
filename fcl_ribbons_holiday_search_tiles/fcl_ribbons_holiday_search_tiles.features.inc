<?php
/**
 * @file
 * fcl_ribbons_holiday_search_tiles.features.inc
 */

/**
 * Implements hook_paragraphs_info().
 *
 * @see fcl_ribbons_holiday_search_tiles.module:5
 */
function fcl_ribbons_holiday_search_tiles_paragraphs_info() {
  $items = array(
    'holiday_search_tiles' => array(
      'name' => 'Holiday Tiles (filtered by holiday type)',
      'bundle' => 'holiday_search_tiles',
      'locked' => '1',
    ),
  );
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function fcl_ribbons_holiday_search_tiles_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function fcl_ribbons_holiday_search_tiles_image_default_styles() {
  $styles = array();

  // Exported image style: holiday_search_tile.
  $styles['holiday_search_tile'] = array(
    'label' => 'Holiday Search Tile',
    'effects' => array(
      22 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 660,
          'height' => 418,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}
