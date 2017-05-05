<?php
/**
 * @file
 * fcl_ribbons_flights_product_listing.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fcl_ribbons_flights_product_listing_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // 'paragraphs_item-flights_product_listing-field_productlist_flts_airlns'.
  $field_instances['paragraphs_item-flights_product_listing-field_productlist_flts_airlns'] = array(
    'bundle' => 'flights_product_listing',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'To limit results to selected airlines, enter one or more airline names, separated by commas, for example: British Airways, Easyjet, Flying Monkeys. Airline names are not case sensitive, but you should double-check that flights for the correct airline(s) are being shown after you save your page and view it. <a href="/admin/structure/taxonomy_manager/voc/fcl_product_supplier" target="_blank">Click here to see a list of airline names</a>.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
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
    'field_name' => 'field_productlist_flts_airlns',
    'label' => 'Only show flights from these airlines',
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
      'weight' => 33,
    ),
  );

  // Exported field_instance:
  // 'paragraphs_item-flights_product_listing-field_productlist_flts_classes'.
  $field_instances['paragraphs_item-flights_product_listing-field_productlist_flts_classes'] = array(
    'bundle' => 'flights_product_listing',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Tick the classes of flights which you want to show in this list. If no classes are ticked, all classes of flights will be shown.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
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
    'field_name' => 'field_productlist_flts_classes',
    'label' => 'Flight class(es)',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 35,
    ),
  );

  // Exported field_instance:
  // 'paragraphs_item-flights_product_listing-field_productlist_flts_dest'.
  $field_instances['paragraphs_item-flights_product_listing-field_productlist_flts_dest'] = array(
    'bundle' => 'flights_product_listing',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '<p>Enter the name of one or more destinations which you would like to show flights for, either one per line, or in a comma-separated list.</p>
<p>For example:</p>
<pre>New York
Dubai
Lapland</pre>
<p>... or:</p>
<pre>New York, Dubai, Lapland</pre>',
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
    'field_name' => 'field_productlist_flts_dest',
    'label' => 'Destination(s)',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 32,
    ),
  );

  // Exported field_instance:
  // 'paragraphs_item-flights_product_listing-field_productlist_flts_expts'.
  $field_instances['paragraphs_item-flights_product_listing-field_productlist_flts_expts'] = array(
    'bundle' => 'flights_product_listing',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '<p>Enter the name of one or more departure points which you would like to show flights for, either one per line, or in a comma-separated list.</p>
<p>For example:</p>
<pre>London Heathrow
London Gatwick
Button Moon</pre>
<p>... or:</p>
<pre>London Heathrow, London Gatwick, Button Moon</pre>',
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
    'field_name' => 'field_productlist_flts_expts',
    'label' => 'Departure point(s)',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 31,
    ),
  );

  // Exported field_instance:
  // 'paragraphs_item-flights_product_listing-field_productlist_max_results'.
  $field_instances['paragraphs_item-flights_product_listing-field_productlist_max_results'] = array(
    'bundle' => 'flights_product_listing',
    'default_value' => array(
      0 => array(
        'value' => -1,
      ),
    ),
    'deleted' => 0,
    'description' => 'You can set a limit to the number of flights shown here. If you choose "default", the global maximum limit set by your administrator will be used.',
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
    'field_name' => 'field_productlist_max_results',
    'label' => 'Maximum number of results to show',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 34,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('<p>Enter the name of one or more departure points which you would like to show flights for, either one per line, or in a comma-separated list.</p>
<p>For example:</p>
<pre>London Heathrow
London Gatwick
Button Moon</pre>
<p>... or:</p>
<pre>London Heathrow, London Gatwick, Button Moon</pre>');
  t('<p>Enter the name of one or more destinations which you would like to show flights for, either one per line, or in a comma-separated list.</p>
<p>For example:</p>
<pre>New York
Dubai
Lapland</pre>
<p>... or:</p>
<pre>New York, Dubai, Lapland</pre>');
  t('Departure point(s)');
  t('Destination(s)');
  t('Flight class(es)');
  t('Maximum number of results to show');
  t('Only show flights from these airlines');
  t('Tick the classes of flights which you want to show in this list. If no classes are ticked, all classes of flights will be shown.');
  t('To limit results to selected airlines, enter one or more airline codes, separated by commas, for example: BA, EZY, ABC, XYZ.');
  t('You can set a limit to the number of flights shown here. If you choose "default", the global maximum limit set by your administrator will be used.');

  return $field_instances;
}
