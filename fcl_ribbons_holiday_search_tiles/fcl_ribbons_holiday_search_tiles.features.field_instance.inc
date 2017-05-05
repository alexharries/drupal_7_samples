<?php
/**
 * @file
 * fcl_ribbons_holiday_search_tiles.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fcl_ribbons_holiday_search_tiles_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // 'paragraphs_item-holiday_search_tiles-field_holiday_tiles_more_results'.
  $field_instances['paragraphs_item-holiday_search_tiles-field_holiday_tiles_more_results'] = array(
    'bundle' => 'holiday_search_tiles',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Do you want to display a link below these holiday tiles leading to more search results (or, to another URL of your choosing - for example, if you have a campaign landing page)?',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
      'paragraphs_editor_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'paragraphs_item',
    'fences_wrapper' => 'div_div_div',
    'field_name' => 'field_holiday_tiles_more_results',
    'label' => 'Show a "More results" link?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 33,
    ),
  );

  // Exported field_instance:
  // 'paragraphs_item-holiday_search_tiles-field_holiday_tiles_more_text'.
  $field_instances['paragraphs_item-holiday_search_tiles-field_holiday_tiles_more_text'] = array(
    'bundle' => 'holiday_search_tiles',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'This is the text to be shown on the "More Results" link. If you leave this empty, the link will default to <em>Show more results</em>.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'paragraphs_editor_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'paragraphs_item',
    'fences_wrapper' => 'div_div_div',
    'field_name' => 'field_holiday_tiles_more_text',
    'label' => 'More Results link text',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 34,
    ),
  );

  // Exported field_instance:
  // 'paragraphs_item-holiday_search_tiles-field_holiday_tiles_more_url'.
  $field_instances['paragraphs_item-holiday_search_tiles-field_holiday_tiles_more_url'] = array(
    'bundle' => 'holiday_search_tiles',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '<p>Optionally, enter a custom URL of the page to send visitors to when they click the "More Results" link. You should enter the URL without the domain. Keep the leading slash.</p>
<p>For example:</p>
<ul>
<li>To link to <em>http://www.flightcentre.co.uk/hello/world</em> enter <em>/hello/world</em></li>
<li>To link to <em>http://www.flightcentre.co.uk/holidays/search?_processed=1</em> enter <em>/holidays/search?_processed=1</em></li>
</ul>
<p>If you leave this field empty, the link will be automatically generated to point to a search results page to show more of these holidays.</p>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'paragraphs_editor_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'paragraphs_item',
    'fences_wrapper' => 'div_div_div',
    'field_name' => 'field_holiday_tiles_more_url',
    'label' => 'More Results link URL',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 35,
    ),
  );

  // Exported field_instance:
  // 'paragraphs_item-holiday_search_tiles-field_holiday_tiles_results'.
  $field_instances['paragraphs_item-holiday_search_tiles-field_holiday_tiles_results'] = array(
    'bundle' => 'holiday_search_tiles',
    'default_value' => array(
      0 => array(
        'value' => 6,
      ),
    ),
    'deleted' => 0,
    'description' => 'Choose the maximum number of tiles to be shown.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
      'paragraphs_editor_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'paragraphs_item',
    'fences_wrapper' => 'div_div_div',
    'field_name' => 'field_holiday_tiles_results',
    'label' => 'Number of tiles to show',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 32,
    ),
  );

  // Exported field_instance:
  // 'paragraphs_item-holiday_search_tiles-field_holiday_tiles_types'.
  $field_instances['paragraphs_item-holiday_search_tiles-field_holiday_tiles_types'] = array(
    'bundle' => 'holiday_search_tiles',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter one or more holiday types, separated with commas. For example: <em>Beach, Luxury, Special Offers, Wildlife & Nature</em>. If you leave this field empty, all holiday types will be shown.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'paragraphs_editor_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'paragraphs_item',
    'fences_wrapper' => 'div_div_div',
    'field_name' => 'field_holiday_tiles_types',
    'label' => 'Holiday Types',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 31,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('<p>Enter the URL of the page to send visitors to when they click the "More Results" link. You should enter the URL without the domain. Keep the leading slash.</p>
<p>For example:</p>
<ul>
<li>To link to <em>http://www.flightcentre.co.uk/hello/world</em> enter <em>/hello/world</em></li>
<li>To link to <em>http://www.flightcentre.co.uk/holidays/search?_processed=1</em> enter <em>/holidays/search?_processed=1</em></li>
</ul>
<p>If you leave this field empty, the link will be automatically generated to point to a search results page to show more of these holidays.</p>');
  t('Choose the maximum number of tiles to be shown.');
  t('Do you want to display a link below these holiday tiles leading to more search results (or, to another URL of your choosing - for example, if you have a campaign landing page)?');
  t('Enter one or more holiday types, separated with commas. For example: <em>Beach, Luxury, Special Offers, Wildlife & Nature</em>. If you leave this field empty, all holiday types will be shown.');
  t('Holiday Types');
  t('More Results link URL');
  t('More Results link text');
  t('Number of tiles to show');
  t('Show a "More results" link?');
  t('This is the text to be shown on the "More Results" link. If you leave this empty, the link will default to <em>Show more results</em>.');

  return $field_instances;
}
