<?php
/**
 * @file
 * fcl_fcuk_communication_pod_paragraph.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fcl_fcuk_communication_pod_paragraph_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // 'paragraphs_item-communication_pod-field_paragraphs_comm_pod'.
  $field_instances['paragraphs_item-communication_pod-field_paragraphs_comm_pod'] = array(
    'bundle' => 'communication_pod',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Choose which communication pod you would like to show.',
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
    'field_name' => 'field_paragraphs_comm_pod',
    'label' => 'Communication Pod to show',
    'required' => 1,
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
  t('Choose which communication pod you would like to show.');
  t('Communication Pod to show');

  return $field_instances;
}
