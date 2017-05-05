<?php
/**
 * @file
 * fcl_ribbons_holiday_search_tiles.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function fcl_ribbons_holiday_search_tiles_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_holiday_tiles_more_results|paragraphs_item|holiday_search_tiles|form';
  $field_group->group_name = 'group_holiday_tiles_more_results';
  $field_group->entity_type = 'paragraphs_item';
  $field_group->bundle = 'holiday_search_tiles';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'More Results Link Settings',
    'weight' => '36',
    'children' => array(
      0 => 'field_holiday_tiles_more_text',
      1 => 'field_holiday_tiles_more_url',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-holiday-tiles-more-results field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_holiday_tiles_more_results|paragraphs_item|holiday_search_tiles|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('More Results Link Settings');

  return $field_groups;
}
