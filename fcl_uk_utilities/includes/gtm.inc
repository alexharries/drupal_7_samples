<?php
/**
 * @file
 * Gooogle Tag Manager functions.
 */

/**
 * Centralised function for firing a GTM Tracking event.
 *
 * @param array $params
 *   An array of params to pass through to GTM.track(). Must contain "event".
 *
 * @return array
 * JS render array, ready to be added to ['#attach']['js'][].
 *
 * @throws \Exception
 */
function fcl_uk_utilities_event_js($params) {
  if (empty($params['event'])) {
    throw new Exception('"event" key not found.');
  }
  $collate = [];
  foreach ($params as $key => $value) {
    $collate[] = $key . ': "' . $value . '"';
  }
  return [
    'data' => 'GTM.track({' . implode(', ', $collate) . '});',
    'type' => 'inline',
    'scope' => 'footer',
  ];
}
