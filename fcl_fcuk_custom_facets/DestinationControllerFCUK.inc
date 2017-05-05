<?php
/**
 * @file
 * Class for loading Destination entities.
 */

class DestinationControllerFCUK extends DestinationController {

  /**
   * Grab the destination terms associated with this IATA.
   *
   * @param string $iata
   *   the IATA code.
   *
   * @return array
   *   an array of term IDs that match.
   */
  public function getDestinationTermsByIATA($iata) {
    $destinations = array();

    $pq = new PlaceEntityFieldQuery();
    $pq->airportOnly()
      ->propertyCondition('location_code', $iata)
      ->fieldCondition('field_fcl_destination', 'tid', 'NULL', '!=');
    $place_results = $pq->execute();

    if (!isset($place_results['place'])) {
      // There are no matching airports, check if this is a multi-code like LON.
      $pq = new PlaceEntityFieldQuery();
      $pq->airportOnly()
        ->propertyCondition('multi_airport_code', $iata)
        ->fieldCondition('field_fcl_destination', 'tid', 'NULL', '!=');
      $place_results = $pq->execute();
    }

    if (isset($place_results['place'])) {
      $places = place_load_multiple(array_keys($place_results['place']));
      foreach ($places as $place) {
        $pw = entity_metadata_wrapper('place', $place);
        $terms = $pw->field_fcl_destination->value();
        foreach ($terms as $term) {
          $destinations[$term->tid] = $term->tid;
        }
      }
    }
    return $destinations;
  }
}
