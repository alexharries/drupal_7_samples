<?php

/**
 * @file
 * Controller for the Communication Pod entity.
 *
 * Also @see fcl_fcuk_communication_pods.entity.inc
 */
class CommunicationPodController extends DrupalDefaultEntityController {
  /**
   * Save handler for Communication Pod entities.
   *
   * @param CommunicationPod $pod
   *
   * @return CommunicationPod entity|False
   *   The Communication Pod, saved, or FALSE if something went askew.
   */
  public function save($pod) {
    // If the pod is new, it won't have an id; otherwise, populate the
    // primary key array.
    $primary_keys = [];
    $pod_is_new = empty($pod->id);

    if (!$pod_is_new) {
      $primary_keys[] = COMMUNICATION_POD_ID_FIELD;
    }

    // Serialise the pod's arrays.
    fcl_fcuk_communication_pods_serialize_unserialize_values($pod, 'serialize');

    // Attempt to save. Try and handle failures in a semi-gracefully way.
    if ($write_result = drupal_write_record(COMMUNICATION_POD_SCHEMA_NAME, $pod, $primary_keys)) {
      if ($pod_is_new) {
        field_attach_insert(COMMUNICATION_POD_ENTITY_NAME, $pod);
        $hook_to_invoke = 'entity_insert';
      }
      else {
        field_attach_update(COMMUNICATION_POD_ENTITY_NAME, $pod);
        $hook_to_invoke = 'entity_update';
      }

      // Invoke hook_entity_<something> hooks.
      module_invoke_all($hook_to_invoke, $pod, COMMUNICATION_POD_ENTITY_NAME);

      return $pod;
    }

    return FALSE;
  }

  /**
   * Load handler for Communication Pod entities.
   *
   * @param array $ids
   * @param array $conditions
   *
   * @return \CommunicationPods
   */
//  public function load(array $ids = [], array $conditions = []) {
//    $monkey = TRUE;
//  }
}
