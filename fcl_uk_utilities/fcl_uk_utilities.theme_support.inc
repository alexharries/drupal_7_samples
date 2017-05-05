<?php
/**
 * @file
 * Module hook implementations for the fcl_uk_fc theme.
 *
 * Unfortunately Drupal 7 treats modules and themes differently. That means that
 * themes can't implement module hooks (permissions, image styles, etc). In this
 * file we will implement all the hooks necessary for the fcl_uk_fc theme.
 *
 * DO NOT USE THIS FILE FOR ANYTHING OTHER THAN SUPPORTING THE fcl_uk_fc THEME.
 */

/**
 * Implements hook_permission().
 *
 * This implementation is for the fcl_uk_fc theme only. Most of the time
 * permissions should be defined in their appropriate module.
 */
function fcl_uk_utilities_permission() {
  return array(
    'display grid overlay' => array(
      'title' => t('Display Grid Overlay'),
      'description' => t('Press G for Hashigrid.'),
    ),
  );
}

/**
 * Implements hook_ctools_plugin_api().
 */
function fcl_uk_utilities_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "flexslider" && $api == "flexslider_default_preset") {
    return array("version" => "1");
  }
  if ($module == "picture" && $api == "default_picture_mapping") {
    return array("version" => "2");
  }
}

/**
 * Implements hook_image_default_styles().
 *
 * Here is a definition of all the styles that the theme expects. This way we
 * can ensure they are not undefined. By default they will be identical to
 * "original". We'll apply effects later.
 *
 * @see fcl_uk_utilities_image_styles_alter()
 */
function fcl_uk_utilities_image_default_styles() {
  $styles = [];

  $styles['fcluk_hero_desktop'] = [
    'label' => t('Hero Size: Desktop'),
    'effects' => [],
  ];

  $styles['fcluk_hero_desktop_large'] = [
    'label' => t('Hero Size: Desktop (Large Variant)'),
    'effects' => [],
  ];

  $styles['fcluk_hero_desktop_small'] = [
    'label' => t('Hero Size: Desktop (Small Variant)'),
    'effects' => [],
  ];

  $styles['fcluk_hero_tablet'] = [
    'label' => t('Hero Size: Tablet'),
    'effects' => [],
  ];

  $styles['fcluk_hero_mobile'] = [
    'label' => t('Hero Size: Mobile'),
    'effects' => [],
  ];

  $styles['fcluk_product_thumbnail'] = [
    'label' => t('Product Image: Thumbnail'),
    'effects' => [],
  ];

  $styles['fcluk_product_half'] = [
    'label' => t('Product Image: Half'),
    'effects' => [],
  ];

  $styles['fcluk_product_hero'] = [
    'label' => t('Product Image: Hero'),
    'effects' => [],
  ];

  return $styles;
}

/**
 * Implements hook_image_styles_alter().
 *
 * As an alter hook, technically this could be added in the theme, but then it
 * would be ignored while in the admin theme.
 *
 * By definining our effects in an alter hook, we can ensure they are not
 * removed in the UI. Any UI changes will be overridden by our alter.
 *
 * If you want to provide an effect as a default only, but allow it to be
 * overridden, you can do a check on the storage parameter.
 */
function fcl_uk_utilities_image_styles_alter(&$styles) {
  $effects = [];
  // Hero styles.
  $effects['fcluk_hero_desktop'] = [
    'name' => 'dimensionactions_switcher',
    'data' => array(
      'width' => 1000,
      'height' => '',
      'smaller' => 'fcluk_hero_desktop_small',
      'larger' => 'fcluk_hero_desktop_large',
    ),
  ];

  $effects['fcluk_hero_desktop_large'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 1920,
      'height' => 432,
    ),
  ];

  $effects['fcluk_hero_desktop_small'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 960,
      'height' => 384,
    ),
  ];

  $effects['fcluk_hero_tablet'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 959,
      'height' => 300,
    ),
  ];

  $effects['fcluk_hero_mobile'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 633,
      'height' => 296,
    ),
  ];

  // Image Grid styles.
  $effects['fcluk_image_grid_large'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 476,
      'height' => 332,
    ),
  ];

  $effects['fcluk_image_grid_large_mobile'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 280,
      'height' => 195,
    ),
  ];

  $effects['fcluk_image_grid_small'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 234,
      'height' => 162,
    ),
  ];

  $effects['fcluk_image_grid_small_mobile'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 134,
      'height' => 93,
    ),
  ];

  $effects['fcluk_image_grid_wide'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 476,
      'height' => 162,
    ),
  ];

  $effects['fcluk_image_grid_wide_mobile'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 280,
      'height' => 95,
    ),
  ];

  // Banner styles.
  $effects['fcluk_banner_sidebar'] = [
    'name' => 'image_scale',
    'data' => array(
      'width' => '230',
      'height' => '',
      'upscale' => 1,
    ),
  ];

  // Product styles.
  $effects['fcluk_product_thumbnail'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 200,
      'height' => 130,
    ),
  ];

  $effects['fcluk_product_half'] = [
    'name' => 'focal_point_scale_and_crop',
    'data' => array(
      'width' => 320,
      'height' => 180,
    ),
  ];

  $effects['fcluk_product_hero'] = [
    'name' => 'dimensionactions_switcher',
    'data' => array(
      'width' => 1000,
      'height' => '',
      // @TODO: For now we are just reusing the existing styles.
      'smaller' => 'fcluk_hero_desktop_small',
      'larger' => 'fcluk_hero_desktop_large',
    ),
  ];

  // Apply the effects only to styles that have already been defined.
  foreach ($effects as $style_name => $effect) {
    if (!empty($styles[$style_name])) {
      $effect['weight'] = 0;
      $definition = image_effect_definition_load($effect['name']);
      if (empty($definition)) {
        watchdog('fcl_uk_utilities.theme_support', 'Could not load effect @effect', ['@effect' => $effect['name']], WATCHDOG_ERROR);
        continue;
      }
      $effect = array_merge($definition, $effect);
      $styles[$style_name]['effects'] = [$effect];
      // Don't allow database overrides.
      $styles[$style_name]['storage'] = IMAGE_STORAGE_DEFAULT;
    }
  }
}

/**
 * Implements hook_default_picture_mapping().
 */
function fcl_uk_utilities_default_picture_mapping() {
  $export = array();

  $picture_mapping = new PictureMapping();
  $picture_mapping->disabled = FALSE;
  $picture_mapping->api_version = 2;
  $picture_mapping->label = 'Hero Map';
  $picture_mapping->machine_name = 'fcluk_hero_map';
  $picture_mapping->breakpoint_group = 'fcl_uk_fc';
  $picture_mapping->mapping = array(
    'breakpoints.theme.fcl_uk_fc.tv' => array(
      '1x' => array(
        'mapping_type' => '_none',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.desktop' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_hero_desktop',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.tablet' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_hero_tablet',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.mobile' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_hero_mobile',
      ),
      '2x' => array(
        'mapping_type' => '_none',
      ),
    ),
  );
  $export[$picture_mapping->machine_name] = $picture_mapping;

  $picture_mapping = new PictureMapping();
  $picture_mapping->disabled = FALSE;
  $picture_mapping->api_version = 2;
  $picture_mapping->label = 'Product Image Map: Hero';
  $picture_mapping->machine_name = 'fcluk_product_hero';
  $picture_mapping->breakpoint_group = 'fcl_uk_fc';
  $picture_mapping->mapping = array(
    'breakpoints.theme.fcl_uk_fc.tv' => array(
      '1x' => array(
        'mapping_type' => '_none',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.desktop' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_hero_desktop',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.tablet' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_hero_tablet',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.mobile' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_hero_mobile',
      ),
      '2x' => array(
        'mapping_type' => '_none',
      ),
    ),
  );
  $export[$picture_mapping->machine_name] = $picture_mapping;

  $picture_mapping = new PictureMapping();
  $picture_mapping->disabled = FALSE;
  $picture_mapping->api_version = 2;
  $picture_mapping->label = 'Image Grid Map: Large';
  $picture_mapping->machine_name = 'fcluk_image_grid_map_large';
  $picture_mapping->breakpoint_group = 'fcl_uk_fc';
  $picture_mapping->mapping = array(
    'breakpoints.theme.fcl_uk_fc.tv' => array(
      '1x' => array(
        'mapping_type' => '_none',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.desktop' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_large',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.tablet' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_large',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.mobile' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_large_mobile',
      ),
      '2x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_large_mobile',
      ),
    ),
  );
  $export[$picture_mapping->machine_name] = $picture_mapping;

  $picture_mapping = new PictureMapping();
  $picture_mapping->disabled = FALSE;
  $picture_mapping->api_version = 2;
  $picture_mapping->label = 'Image Grid Map: Small';
  $picture_mapping->machine_name = 'fcluk_image_grid_map_small';
  $picture_mapping->breakpoint_group = 'fcl_uk_fc';
  $picture_mapping->mapping = array(
    'breakpoints.theme.fcl_uk_fc.tv' => array(
      '1x' => array(
        'mapping_type' => '_none',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.desktop' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_small',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.tablet' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_small',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.mobile' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_small_mobile',
      ),
      '2x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_small_mobile',
      ),
    ),
  );
  $export[$picture_mapping->machine_name] = $picture_mapping;

  $picture_mapping = new PictureMapping();
  $picture_mapping->disabled = FALSE;
  $picture_mapping->api_version = 2;
  $picture_mapping->label = 'Image Grid Map: Wide';
  $picture_mapping->machine_name = 'fcluk_image_grid_map_wide';
  $picture_mapping->breakpoint_group = 'fcl_uk_fc';
  $picture_mapping->mapping = array(
    'breakpoints.theme.fcl_uk_fc.tv' => array(
      '1x' => array(
        'mapping_type' => '_none',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.desktop' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_wide',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.tablet' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_wide',
      ),
    ),
    'breakpoints.theme.fcl_uk_fc.mobile' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_wide_mobile',
      ),
      '2x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fcluk_image_grid_wide_mobile',
      ),
    ),
  );
  $export[$picture_mapping->machine_name] = $picture_mapping;

  return $export;
}

/**
 * Implements hook_flexslider_default_presets().
 */
function fcl_uk_utilities_flexslider_default_presets() {
  $export = array();

  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'fcluk_hero_slider';
  $preset->title = 'Hero Slider';
  $preset->theme = 'classic';
  $preset->options = array(
    'namespace' => 'flex-',
    'selector' => '.slides > li',
    'easing' => 'swing',
    'direction' => 'horizontal',
    'reverse' => 0,
    'smoothHeight' => 0,
    'startAt' => '0',
    'animationSpeed' => '600',
    'initDelay' => '0',
    'useCSS' => 1,
    'touch' => 1,
    'video' => 0,
    'keyboard' => 1,
    'multipleKeyboard' => 0,
    'mousewheel' => 0,
    'controlsContainer' => '.flex-control-nav-container',
    'sync' => '',
    'asNavFor' => '',
    'itemWidth' => '0',
    'itemMargin' => '0',
    'minItems' => '0',
    'maxItems' => '0',
    'move' => '0',
    'animation' => 'fade',
    'slideshow' => 1,
    'slideshowSpeed' => '7000',
    'directionNav' => 1,
    'controlNav' => '1',
    'prevText' => 'Previous',
    'nextText' => 'Next',
    'pausePlay' => 0,
    'pauseText' => 'Pause',
    'playText' => 'Play',
    'randomize' => 0,
    'thumbCaptions' => 0,
    'thumbCaptionsBoth' => 0,
    'animationLoop' => 1,
    'pauseOnAction' => 1,
    'pauseOnHover' => 0,
    'manualControls' => '',
  );
  $export[$preset->name] = $preset;

  return $export;
}
