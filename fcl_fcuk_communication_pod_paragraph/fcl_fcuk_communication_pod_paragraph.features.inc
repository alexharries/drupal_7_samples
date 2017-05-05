<?php
/**
 * @file
 * fcl_fcuk_communication_pod_paragraph.features.inc
 */

/**
 * Implements hook_paragraphs_info().
 */
function fcl_fcuk_communication_pod_paragraph_paragraphs_info() {
  $items = array(
    'communication_pod' => array(
      'name' => 'Communication Pod',
      'bundle' => 'communication_pod',
      'locked' => '1',
    ),
  );
  return $items;
}
