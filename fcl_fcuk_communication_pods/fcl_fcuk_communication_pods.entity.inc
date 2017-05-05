<?php

/**
 * @file
 * Entity functions for the Communication Pods module.
 *
 * Also @see fcl_fcuk_communication_pods.controller.inc
 */

/**
 * Load one or more communication pods.
 *
 * Pods are loaded from the database in the first instance; if not found,
 * we look in the /pods/ directory in the module for file-defined pods.
 *
 * @param $pods_to_load
 *   An array containing one or more Communication Pod machine names, or FALSE
 *   to load all.
 *
 * @param array $conditions
 *
 * @param bool $reset
 *   Resets the entity cache if TRUE.
 *
 * @return array
 *   The loaded entities.
 */
function fcl_fcuk_communication_pods_load_multiple($pods_to_load = FALSE, array $conditions = [], $reset = FALSE) {
  // We need to convert the machine names to entity IDs, if we have any
  // machine names.
  $pods_to_load_by_id = FALSE;
  if (is_array($pods_to_load)) {
    $pods_to_load_by_id = array_values(fcl_fcuk_communication_pods_machine_name_to_entity_id($pods_to_load));
  }

  $loaded_communication_pods_by_id = entity_load(COMMUNICATION_PODS_ENTITY_NAME, $pods_to_load_by_id, $conditions, $reset);

  // Convert the loaded pods array keys to be the pod machine names.
  $loaded_communication_pods = [];

  foreach ($loaded_communication_pods_by_id as $pod_id => $pod_entity) {
    $loaded_communication_pods[$pod_entity->{COMMUNICATION_POD_MACHINE_NAME}] = $pod_entity;
  }

  // If $pods_to_load isn't empty, check for missing pods; if $pods_to_load
  // is empty, then we are loading all the pods on disk.
  $missing_pods_to_load_from_disk = [];

  if (!empty($pods_to_load)) {
    $pods_to_load_by_key = array_flip($pods_to_load);
    $missing_pods_to_load_from_disk = array_diff_key($pods_to_load_by_key, $loaded_communication_pods);

    // Flip the missing pods array back.
    $missing_pods_to_load_from_disk = array_keys($missing_pods_to_load_from_disk);
  }

  if (empty($pods_to_load) || !empty($missing_pods_to_load_from_disk)) {
    $loaded_communication_pods += _fcl_fcuk_communication_pods_load_from_disk($missing_pods_to_load_from_disk, $reset);
  }

  return $loaded_communication_pods;
}

/**
 * Load a single communication pod by its machine name.
 *
 * @param string|null $pod_machine_name
 *   The machine name of the Communication Pod.
 *
 * @param bool $reset
 *   Resets the entity cache if TRUE.
 *
 * @return CommunicationPod|FALSE
 *   If found, returns the Communication Pod entity; if not found,
 *   returns FALSE.
 */
function fcl_fcuk_communication_pods_load($pod_machine_name = NULL, $reset = FALSE) {
  $pod_machine_names = isset($pod_machine_name) ? [$pod_machine_name] : [];

  if ($pods = fcl_fcuk_communication_pods_load_multiple($pod_machine_names, [], $reset)) {
    $pod = reset($pods);

    return $pod;
  }

  return FALSE;
}

/**
 * Saves a Communication Pod to the database.
 *
 * @param CommunicationPod $pod
 *
 * @return CommunicationPod
 *   The saved Communication Pod entity.
 */
function fcl_fcuk_communication_pods_save($pod) {
  return entity_get_controller(COMMUNICATION_PODS_ENTITY_NAME)->save($pod);
}

/**
 * Deletes a Communication Pod from the database.
 *
 * @param CommunicationPod $pod
 *
 * @return bool
 *   False if unsuccessful. True if successful?
 */
function fcl_fcuk_communication_pods_delete($pod) {
  return entity_get_controller(COMMUNICATION_PODS_ENTITY_NAME)->delete([$pod->{COMMUNICATION_POD_MACHINE_NAME}]);
}
