<?php

/**
 * @file
 * API functions for the fcl_fcuk_ajax_search_forms module.
 */

/**
 * Builds an Ajax-updated search form.
 *
 * @param array $form
 *   The Drupal Form API form array.
 *
 * @param string $context
 *   The context, for example "holidays" or "flights".
 */
function fcl_fcuk_ajax_search_forms_get_search_form(&$form, $context) {
  // This element is a required hangover from the Static Forms module because
  // it's not possible to unset travel_details; you end up with an empty
  // render array.
  $form['travel_details'] = [
    '#markup' => '<span class="travel-details"></span>',
  ];

  // Override the form's action URL.
  $form['#action'] = url($context . '/search');

  $form['#attributes']['class'] = array_merge($form['#attributes']['class'], [
    'fcuk-static-forms fcuk-static-forms-' . $context . '-search',
  ]);

  // Set a class which indicates whether the cache is enabled or not. We use
  // this to allow the client-side JS to call the right endpoint; either
  // cached or un-cached.
  $form['#attributes']['data-cache-enabled'] = fcl_fcuk_ajax_search_forms_cache_enabled();

  // Set the context on the form.
  $form['#attributes']['data-context'] = $context;

  $form['search'] = [
    '#type' => 'hidden',
    '#value' => 'static_search',
    '#weight' => FCL_STATIC_FORMS_HIDDEN,
  ];

  $form['heading'] = [
    '#type' => 'markup',
    '#markup' => '<h2 class="form-heading">' . t('Search @context ideas', [
        '@context' => $context,
      ]) . '</h2>',
    '#weight' => 1,
  ];

  // Are we getting Mercury or Procat data?
  if (variable_get('is_mercury_holidays')) {
    $form += fcl_fcuk_ajax_search_forms_get_dropdowns($context);
  }
  else {
    // Is this here a Procat town?
    $form['not_implemented'] = [
      '#type' => 'markup',
      '#markup' => t('<!-- @function called on a site which doesn\'t seem to be using Mercury. Sad face. -->', ['@function' => __FUNCTION__]),
    ];
  }

  $form['submit'] = [
    '#type' => 'submit',
    '#name' => 'op',
    '#value' => t('Find @type', ['@type' => $context]),
    '#weight' => 999,
  ];
}

/**
 * Fetch the drop-downs for a search form.
 *
 * @param string $context
 *   The context, for example "holidays" or "flights".
 *
 * @return array
 *   A render array containing the drop-downs. These drop-downs have
 *   any necessary javascript and CSS attached; for example, we will use
 *   Select2 to improve/awkwardify the user experience.
 */
function fcl_fcuk_ajax_search_forms_get_dropdowns($context) {
  $fields_for_context = fcl_fcuk_ajax_search_forms_get_field_configuration(NULL, NULL, $context);
  $dropdown_option_fields = [];

  if (!empty($fields_for_context)) {
    foreach ($fields_for_context as $field_configuration) {
      $dropdown_option_fields[$field_configuration['url_query_field']] = fcl_fcuk_ajax_search_forms_get_dropdown($field_configuration);
    }
  }
  else {
    $dropdown_option_fields['not_implemented'] = [
      '#type' => 'markup',
      '#markup' => t('<!-- @function(@context) called but @context hasn\'t been implemented yet. Sad face. -->', [
        '@function' => __FUNCTION__,
        '@context' => $context,
      ]),
    ];
  }

  return $dropdown_option_fields;
}

/**
 * Gets a search form element, populated with the correct Solr options.
 *
 * The form element type is based on $field_configuration, which is taken from
 * fcl_fcuk_ajax_search_forms_get_field_configuration().
 *
 * The options for this element come from a Solr search on the Solr field
 * specified in $field_configuration[solr_field].
 *
 * The default_value options are based on the $_GET query parameter whose name
 * is specified in $field_configuration[url_query_field], if it's set.
 *
 * @param array $field_configuration
 *   A field configuration array from
 *   fcl_fcuk_ajax_search_forms_get_field_configuration().
 *
 * @return array
 *   A Drupal Form API array to go into a search form.
 */
function fcl_fcuk_ajax_search_forms_get_dropdown($field_configuration) {
  // Initialise the FAPI array with some defaults.
  // @todo: should this be in a separate function with a hook_alter to allow
  // other modules to alter these defaults?
  $form_api_field_array = [
    '#attached' => [
      'js' => [
        // Add in the Select2 JS library and its config JS file.
        drupal_get_path('module', 'fcl_uk_utilities') . '/js/fcuk-select2-configuration.js' => [],
        drupal_get_path('module', 'fcl_fcuk_ajax_search_forms') . '/js/ajax-search-forms.js' => [],
      ],
      'css' => [],
      'library' => [
        [
          'system',
          'drupal.ajax',
        ],
      ],
      'libraries_load' => [
        ['select2'],
      ],
    ],
  ];

  // Set the field type.
  $form_api_field_array['#type'] = $field_configuration['fapi_type'];

  // Can the field accept multiple values?
  $form_api_field_array['#multiple'] = $field_configuration['fapi_multiple_select'];

  // Set any field title and description.
  if (!empty($field_configuration['title'])) {
    $form_api_field_array['#title'] = $field_configuration['title'];
  }

  if (!empty($field_configuration['description'])) {
    $form_api_field_array['#description'] = $field_configuration['description'];
  }

  // Get the field's options.
  $form_api_field_array['#options'] = fcl_fcuk_ajax_search_forms_get_dropdown_options($field_configuration);

  // Do we have any defaults specified in the URL?
  $form_api_field_array['#default_value'] = fcl_fcuk_ajax_search_forms_get_url_query_values($field_configuration, $form_api_field_array['#options']);

  // Do we have any Javascript or CSS to attach to the field?
  if (!empty($field_configuration['fapi_attached_js'])) {
    $form_api_field_array['#attached']['js'] = $field_configuration['fapi_attached_js'];
  }

  if (!empty($field_configuration['fapi_attached_css'])) {
    $form_api_field_array['#attached']['css'] = $field_configuration['fapi_attached_css'];
  }

  // Apply attributes to the field.
  $form_api_field_array['#attributes'] = $field_configuration['fapi_attributes'];

  // Do we have any classes to apply to the field?
  if (!empty($field_configuration['fapi_classes'])) {
    $form_api_field_array['#attributes']['class'] = $field_configuration['fapi_classes'];
  }

  // Set some data attributes on the element.
  $form_api_field_array['#attributes']['data-ajax-search-forms-enabled'] = TRUE;
  $form_api_field_array['#attributes']['data-cache-enabled'] = fcl_fcuk_ajax_search_forms_cache_enabled();
  $form_api_field_array['#attributes']['data-context'] = $field_configuration['context'];
  $form_api_field_array['#attributes']['data-callback-path'] = FCL_FCUK_AJAX_SEARCH_FORMS_CALLBACK_PATH;

  // Set the placeholder text which can be used by Select2, etc.
  $form_api_field_array['#attributes']['data-placeholder'] = _fcl_fcuk_ajax_search_forms_dropdown_options_get_all_option($field_configuration);

  return $form_api_field_array;
}

/**
 * Gets the field information for a Solr search field.
 *
 * This function returns an array of configuration information about a Solr
 * search field which is used by
 * fcl_fcuk_ajax_search_forms_get_dropdown_options() to build a drop-down list
 * of select options, by performing a Solr search.
 *
 * @param string|null $field_name
 *   The Solr field name.
 *
 * @param string $field_name_type_to_search_for
 *   The type of field to search for; e.g. solr_field or url_query_field.
 *   Defaults to solr_field.
 *
 * @param string|null $context
 *   Optional: the context, e.g. "holidays" or "flights". If not specified, no
 *   filtering of results by context is performed.
 *
 * @return array|FALSE
 *   An array of field information, if found, or FALSE if not.
 */
function fcl_fcuk_ajax_search_forms_get_field_configuration($field_name = NULL, $field_name_type_to_search_for = 'solr_field', $context = NULL) {
  $all_fields_config = &drupal_static(__FUNCTION__ . '_all_fields_config');
  $fields_by_context = &drupal_static(__FUNCTION__ . '_fields_by_context', []);

  // Get the field information if we haven't already loaded it.
  if (is_null($all_fields_config)) {
    $all_fields_config = [
      // Destination.
      [
        // The field's name in the Solr index. When we build up the list
        // of select options, we will actually build the string from the contents
        // of im_mercury_holiday_destinations_tids, which is an array of one
        // or more destination entity IDs, starting with this destination,
        // and ending with World or somesuch (which we will ignore).
        'solr_field' => 'ss_mercury_holiday_key_destination',

        // The field's name when it appears in a URL query parameter,
        // e.g. ?destination=button-moon.
        'url_query_field' => 'destination',

        // The label for the field.
        'title' => t('Destination'),

        // The entity type.
        'entity_type' => 'mercury_holiday',

        // The search context, e.g. "holidays", "flights", etc.
        'context' => 'holidays',

        // Any additional values to pass into Solr's field query or field
        // list parameters.
        'solr_field_query_array' => [
          // Only show published entities.
          // @todo: Do we need to detect if we're on RTWE or FCUK and use
          // bs_mercury_holiday_rtweuk_publish_status or
          // bs_mercury_holiday_fcuk_publish_status accordingly here?
          'bs_mercury_holiday_fcuk_publish_status:1',
        ],

        // Any additional field values to get with the query. For this field,
        // we will use hook_fcl_fcuk_static_forms_dropdown_options_alter to
        // add this hierarchy into the option's label.
        'solr_field_list_array' => [
          'im_mercury_holiday_destinations_tids',
          'im_mercury_holiday_segments_destinations_tids',
        ],

        // When getting a list of values, which other Solr fields will be used
        // to filter these values? For example, if the URL contains a
        // 'destination' query parameter, we will only get a list of holiday
        // types which can be found at the destination(s) specified by the
        // 'destination' query parameter, which maps to the Solr
        // 'ss_mercury_holiday_key_destination' field (for Mercury holidays).
        // 'inherited_filters' => [],

        // What function should we call which converts the results objects into
        // a string?
        'process_functions' => [
          'fcl_fcuk_ajax_search_forms_dropdown_options_process_destinations',
          'fcl_fcuk_ajax_search_forms_dropdown_options_natcasesort',
        ],

        // If we want to add an "All" option in at the top of the list of
        // results, we set 'all_results' to either TRUE to show the default "All" text,
        // or a value such as t('All holiday destinations') to show a custom
        // message.
        'all_results' => t('All Destinations'),

        // If we want to show a "no results found" entry instead of an empty
        // select list, set 'no_results' to either TRUE to show the default
        // "No results" text, or a value such as t('No holiday destinations
        // found') to show a custom message.

        // We use "All ..." here because
        // the only time we should be showing a no results message is if
        // Solr and Mercury are out of sync, and in those cases we would
        // rather the customer not see an error but instead try a search,
        // because this is likely to return some results. We hope...
        'no_results' => t('All destinations'),

        // Allow customers to select more than one option in this list. We
        // will finagle the UI with something like Select2.js to make it
        // easier to select multiple destinations.
        // @todo: This is disabled because Select2 doesn't seem to handle
        // multiple selects in a sane manner; if you enable this option,
        // Select2 allows you to select multiple options from this drop-down -
        // which is lovely - but when you submit the form, those values aren't
        // sent in $_REQUEST. Damn.
        // 'fapi_multiple_select' => TRUE,

        // We can specify additional CSS classes to be added to the field.
        // The .ignore class is used to prevent the fsf.fields.js
        // module from storing the drop-down's value in local storage. This
        // avoids weird behaviour such as Select2 drop-downs from appearing
        // blank.

        // @see Drupal.behaviors.StaticFormsFields.getFieldData()
        // (specifically, the line
        // 'field.val(value).data('value', value).attr('data-value', value);'
        // in docroot/web/docroot/sites/all/modules/custom/fcl_lead/modules/fcl_static_forms/assets/fsf.fields.js
        'fapi_classes' => [
          'ignore',
        ],

        // You can set the attributes which will be used on the FAPI element
        // by creating an array in the same format as a Drupal Form API
        // attributes array, as follows:
        //
        // 'fapi_attributes' => [
        //   // Tell Select2 to only hide the search list when there are 20
        //   // or more results.
        //   'data-minimum-results-for-search' => 20,
        //   'data-minimum-input-length' => 2,
        //
        //   // Or you can add other attributes, such as...
        //   'data-monkey' => 'banana',
        //   'data-trevor' => 'turkey-mango',
        // ];
        'fapi_attributes' => [
          // Tell our custom JS to use Select2 in this field.
          'data-use-select2' => TRUE,

          // Tell Select2 to only hide the search list when there are 20
          // or more results.
          'data-minimum-results-for-search' => 20,

          // Require at least two characters to be entered into the Select2
          // search field before showing results.
          'data-minimum-input-length' => 2,
        ],

        // This is an example of how you can add Javascript or CSS to your
        // drop-down. Note that, for JS, the array _key_ is the path to the
        // file.
        // @see https://api.drupal.org/api/drupal/includes%21common.inc/function/drupal_process_attached/7.x
        // @see https://api.drupal.org/comment/62051#comment-62051
        //'fapi_attached_js' => [
        //  // This attaches an internal JS file.
        //  drupal_get_path('module', 'fcl_uk_utilities') . '/js/fcuk-select2-configuration.js' => [],
        //
        //  // This attaches an external JS file.
        //  'https://www.gstatic.com/freebase/suggest/4_2/suggest.min.js' => [
        //    'type' => 'external',
        //  ],
        //],
        //
        //'fapi_attached_css' => [
        //  // This attaches an external CSS file.
        //  'https://www.gstatic.com/freebase/suggest/4_2/suggest.min.css' => [
        //    'type' => 'external',
        //  ],
        //],
      ],

      // Holiday type.
      [
        // The field's name in the Solr index.
        'solr_field' => 'sm_mercury_holiday_type',

        // The field's name when it appears in a URL query parameter,
        // e.g. ?type=trekking.
        'url_query_field' => 'type',

        // The label for the field.
        'title' => t('Type'),

        // The entity type.
        'entity_type' => 'mercury_holiday',

        // The search context, e.g. "holidays", "flights", etc.
        'context' => 'holidays',

        // Any additional values to pass into Solr's field query or field
        // list parameters.
        'solr_field_query_array' => [
          // Only show published entities.
          // @todo: Do we need to detect if we're on RTWE or FCUK and use
          // bs_mercury_holiday_rtweuk_publish_status or
          // bs_mercury_holiday_fcuk_publish_status accordingly here?
          'bs_mercury_holiday_fcuk_publish_status:1',
        ],

        // Any additional field values to get with the query.
        'solr_field_list_array' => [],

        // When getting a list of values, which other Solr fields will be used
        // to filter these values? For example, if the URL contains a
        // 'destination' query parameter, we will only get a list of holiday
        // types which can be found at the destination(s) specified by the
        // 'destination' query parameter, which maps to the Solr
        // 'ss_mercury_holiday_key_destination' field (for Mercury holidays).
        'inherited_filters' => [
          'ss_mercury_holiday_key_destination',
        ],

        // What function should we call which converts the results objects into
        // a string?
        'process_functions' => [
          'fcl_fcuk_ajax_search_forms_dropdown_options_process_types',
          'fcl_fcuk_ajax_search_forms_dropdown_options_natcasesort',
        ],

        // If we want to add an "All" option in at the top of the list of
        // results, we set 'all_results' to either TRUE to show the default "All" text,
        // or a value such as t('All holiday destinations') to show a custom
        // message.
        'all_results' => t('All Holiday Types'),

        // If we want to show a "no results found" entry instead of an empty
        // select list, set 'no_results' to either TRUE to show the default
        // "No results" text, or a value such as t('No holiday destinations
        // found') to show a custom
        // message.

        // We use "All ..." here because
        // the only time we should be showing a no results message is if
        // Solr and Mercury are out of sync, and in those cases we would
        // rather the customer not see an error but instead try a search,
        // because this is likely to return some results. We hope...
        'no_results' => t('All holiday types'),

        // We can specify additional CSS classes to be added to the field.
        // The .ignore class is used to prevent the fsf.fields.js
        // module from storing the drop-down's value in local storage. This
        // avoids weird behaviour such as Select2 drop-downs from appearing
        // blank.

        // @see Drupal.behaviors.StaticFormsFields.getFieldData()
        // (specifically, the line
        // 'field.val(value).data('value', value).attr('data-value', value);'
        // in docroot/web/docroot/sites/all/modules/custom/fcl_lead/modules/fcl_static_forms/assets/fsf.fields.js
        'fapi_classes' => [
          'ignore',
        ],

        // You can set the attributes which will be used on the FAPI element
        // by creating an array in the same format as a Drupal Form API
        // attributes array, as follows:
        //
        // 'fapi_attributes' => [
        //   // Tell Select2 to only hide the search list when there are 20
        //   // or more results.
        //   'data-minimum-results-for-search' => 20,
        //   'data-minimum-input-length' => 2,
        //
        //   // Or you can add other attributes, such as...
        //   'data-monkey' => 'banana',
        //   'data-trevor' => 'turkey-mango',
        // ];

        // This is an example of how you can add Javascript or CSS to your
        // drop-down. Note that, for JS, the array _key_ is the path to the
        // file.
        // @see https://api.drupal.org/api/drupal/includes%21common.inc/function/drupal_process_attached/7.x
        // @see https://api.drupal.org/comment/62051#comment-62051
        //'fapi_attached_js' => [
        //  // This attaches an internal JS file.
        //  drupal_get_path('module', 'fcl_uk_utilities') . '/js/fcuk-select2-configuration.js' => [],
        //
        //  // This attaches an external JS file.
        //  'https://www.gstatic.com/freebase/suggest/4_2/suggest.min.js' => [
        //    'type' => 'external',
        //  ],
        //],
        //
        //'fapi_attached_css' => [
        //  // This attaches an external CSS file.
        //  'https://www.gstatic.com/freebase/suggest/4_2/suggest.min.css' => [
        //    'type' => 'external',
        //  ],
        //],
      ],

      // The star rating of the holiday.
      [
        // The field's name in the Solr index.
        'solr_field' => 'fm_mercury_holiday_hotel_star_rating',

        // The field's name when it appears in a URL query parameter, e.g.
        // ?star_rating=4.5.
        'url_query_field' => 'star_rating',

        // The label for the field.
        'title' => t('Rating'),

        // The entity type.
        'entity_type' => 'mercury_holiday',

        // The search context, e.g. "holidays", "flights", etc.
        'context' => 'holidays',

        // Any additional values to pass into Solr's field query or field
        // list parameters.
        'solr_field_query_array' => [
          // Only show published entities.
          // @todo: Do we need to detect if we're on RTWE or FCUK and use
          // bs_mercury_holiday_rtweuk_publish_status or
          // bs_mercury_holiday_fcuk_publish_status accordingly here?
          'bs_mercury_holiday_fcuk_publish_status:1',
        ],

        // Any additional field values to get with the query.
        'solr_field_list_array' => [],

        // When getting a list of values, which other Solr fields will be used
        // to filter these values? For example, if the URL contains a
        // 'destination' query parameter, we will only get a list of holiday
        // types which can be found at the destination(s) specified by the
        // 'destination' query parameter, which maps to the Solr
        // 'ss_mercury_holiday_key_destination' field (for Mercury holidays).
        'inherited_filters' => [
          'ss_mercury_holiday_key_destination',
          'sm_mercury_holiday_type',
        ],

        // What function should we call which converts the results objects into
        // a string?
        'process_functions' => [
          'fcl_fcuk_ajax_search_forms_dropdown_options_process_ratings',
          'fcl_fcuk_ajax_search_forms_dropdown_options_natcasesort_reversed',
        ],

        // If we want to add an "All" option in at the top of the list of
        // results, we set 'all_results' to either TRUE to show the default "All" text,
        // or a value such as t('All holiday destinations') to show a custom
        // message.
        'all_results' => t('All Star Ratings'),

        // If we want to show a "no results found" entry instead of an empty
        // select list, set 'no_results' to either TRUE to show the default
        // "No results" text, or a value such as t('No holiday destinations
        // found') to show a custom
        // message.

        // We use "All ..." here because
        // the only time we should be showing a no results message is if
        // Solr and Mercury are out of sync, and in those cases we would
        // rather the customer not see an error but instead try a search,
        // because this is likely to return some results. We hope...
        'no_results' => t('All star ratings'),

        // We can specify additional CSS classes to be added to the field.
        // The .ignore class is used to prevent the fsf.fields.js
        // module from storing the drop-down's value in local storage. This
        // avoids weird behaviour such as Select2 drop-downs from appearing
        // blank.

        // @see Drupal.behaviors.StaticFormsFields.getFieldData()
        // (specifically, the line
        // 'field.val(value).data('value', value).attr('data-value', value);'
        // in docroot/web/docroot/sites/all/modules/custom/fcl_lead/modules/fcl_static_forms/assets/fsf.fields.js
        'fapi_classes' => [
          'ignore',
        ],

        // You can set the attributes which will be used on the FAPI element
        // by creating an array in the same format as a Drupal Form API
        // attributes array, as follows:
        //
        // 'fapi_attributes' => [
        //   // Tell Select2 to only hide the search list when there are 20
        //   // or more results.
        //   'data-minimum-results-for-search' => 20,
        //   'data-minimum-input-length' => 2,
        //
        //   // Or you can add other attributes, such as...
        //   'data-monkey' => 'banana',
        //   'data-trevor' => 'turkey-mango',
        // ];

        // This is an example of how you can add Javascript or CSS to your
        // drop-down. Note that, for JS, the array _key_ is the path to the
        // file.
        // @see https://api.drupal.org/api/drupal/includes%21common.inc/function/drupal_process_attached/7.x
        // @see https://api.drupal.org/comment/62051#comment-62051
        //'fapi_attached_js' => [
        //  // This attaches an internal JS file.
        //  drupal_get_path('module', 'fcl_uk_utilities') . '/js/fcuk-select2-configuration.js' => [],
        //
        //  // This attaches an external JS file.
        //  'https://www.gstatic.com/freebase/suggest/4_2/suggest.min.js' => [
        //    'type' => 'external',
        //  ],
        //],
        //
        //'fapi_attached_css' => [
        //  // This attaches an external CSS file.
        //  'https://www.gstatic.com/freebase/suggest/4_2/suggest.min.css' => [
        //    'type' => 'external',
        //  ],
        //],
      ],
    ];

    // Add field defaults and sort inherited_filters to make cache IDs
    // consistent.
    // @todo: move this into a separate function?
    // @todo: make $field_configuration_defaults configuraboo/alteraboo?
    $field_configuration_defaults = [
      'solr_field' => NULL,
      'url_query_field' => NULL,
      'title' => NULL,
      'description' => NULL,
      'fapi_type' => 'select',
      'fapi_multiple_select' => FALSE,
      'fapi_attached_js' => [],
      'fapi_attached_css' => [],
      'fapi_classes' => [],
      'fapi_attributes' => [],
      'entity_type' => NULL,
      'context' => NULL,
      'solr_field_query_array' => [],
      'solr_field_list_array' => [],
      'inherited_filters' => [],
      'process_function' => NULL,
      'all_results' => FALSE,
    ];

    foreach ($all_fields_config as &$field_configuration) {
      $field_configuration += $field_configuration_defaults;

      // Sort the inherited filters for cache ID-friendliness; this means
      // we won't end up having two separate cache entries if we have two
      // fields with the same inherited filters, but specified in a
      // different order.
      natcasesort($field_configuration['inherited_filters']);
    }

    // We have to unset $field_configuration as this variable name is used
    // again below. I could have just renamed the variable below to something
    // else, but then there wouldn't be a record of this PHP-gotcha that I
    // just learned. With my brain and everything...
    unset($field_configuration);

    // Implement a hook_fcl_fcuk_static_forms_get_field_configuration_alter.
    drupal_alter('fcl_fcuk_ajax_search_forms_get_field_configuration', $all_fields_config);
    // @todo: Should we cache field information at this point?
  }

  // If $field_name is null, are we being asked to get all fields, or just all
  // fields which match a context?
  if (is_null($field_name)) {
    // If $context is null, we're being asked to return everything we know
    // about all teh fieldz.
    if (is_null($context)) {
      return $all_fields_config;
    }
    // $context isn't null, so we're being asked for a list of fields which
    // apply to a particular context.
    else {
      // We have a context. If it matches any field configurations, assemble
      // them and return.
      if (!array_key_exists($context, $fields_by_context)) {
        // Loop through all defined fields and find the ones whose context
        // matches the search context we've been asked for, for example
        // "holidays" or "flights".
        foreach ($all_fields_config as $field_configuration) {
          if ($field_configuration['context'] == $context) {
            $fields_by_context[$context][$field_configuration['url_query_field']] = $field_configuration;
          }
        }
      }

      return $fields_by_context[$context];
    }
  }

  // Find the field by field name and return, if found, or FALSE if not.
  foreach ($all_fields_config as $field_config) {
    // Are we filtering by entity type?
    if (!is_null($context)) {
      // Yes. Does this field's config match the context we're filtering on?
      if ($field_config['context'] !== $context) {
        // Nyet; skip this option.
        continue;
      }
    }

    // Does the requested field exist in this field's config?
    if (array_key_exists($field_name_type_to_search_for, $field_config)) {
      // Yes. Does its name match the requested field?
      if ($field_config[$field_name_type_to_search_for] == $field_name) {
        // Yep; this is our cookie. Return the field config.
        return $field_config;
      }
    }
  }

  return FALSE;
}

/**
 * Given a field name from a URL query string, get the Solr field name.
 *
 * For example, if you have 'destination' from the URL, this will return
 * 'ss_mercury_holiday_key_destination' from the field configuration info.
 *
 * @param string $url_query_field_name
 *   The URL query field name, e.g. 'destination'.
 *
 * @param string $context
 *   Optional: the context, e.g. holidays. If not specified, no filtering of
 *   results by context is performed.
 *
 * @return string
 *   The Solr field name if found, or the original URL query string if not.
 */
function fcl_fcuk_ajax_search_forms_get_solr_field_name_from_url_query($url_query_field_name, $context = NULL) {
  if ($field_config = fcl_fcuk_ajax_search_forms_get_field_configuration($url_query_field_name, 'url_query_field', $context)) {
    return $field_config['solr_field'];
  }

  return $url_query_field_name;
}

/**
 * Given a Solr field name, get the name to be used in a URL query string.
 *
 * For example, if you have 'ss_mercury_holiday_key_destination' from Solr,
 * this function will return 'destination' from the field configuration info.
 *
 * @param string $solr_field_name
 *   The Solr field name.
 *
 * @param string $entity_type
 *   Optional: the entity type. If not specified, no filtering of results by
 *   entity type is performed.
 *
 * @return string
 *   The URL query string name if found, or the original field name if not.
 */
function fcl_fcuk_ajax_search_forms_get_url_query_name_from_solr_field($solr_field_name, $entity_type = NULL) {
  if ($field_config = fcl_fcuk_ajax_search_forms_get_field_configuration($solr_field_name, 'solr_field', $entity_type)) {
    return $field_config['url_query_field'];
  }

  return $solr_field_name;
}

/**
 * Gets comma-separated values for a query parameter from $_GET, if set.
 *
 * @param string $field_configuration
 *   The URL query field name, e.g. 'destinations'.
 *
 * @param array $dropdown_options
 *   The list of dropdown options. If provided, we use this to see if the
 *   raw value of $_GET matches one of the drop-down options, even if it
 *   includes commas. This allows us to handle the case where we munge
 *   a destination and all its hierarchy into a comma-separated string so
 *   we can differentiate between New Malden, UK and New Malden, France.
 *
 * @param bool $always_get_multiple
 *   If $field_configuration is a single-value field, this function will
 *   usually only return a single value. Set this to TRUE to get all values.
 *
 * @return array
 *   An array of values, if the URL contained a $field_configuration query
 *   parameter. If not found or the URL query parameter was empty, an empty
 *   array is returned.
 */
function fcl_fcuk_ajax_search_forms_get_url_query_values($field_configuration, array $dropdown_options = [], $always_get_multiple = FALSE) {
  // Initialise our return value as an array if the field allows multi-selects,
  // or NULL if not.
  $values_from_url = ($always_get_multiple || $field_configuration['fapi_multiple_select']) ? [] : NULL;

  // Does the URL contain a $_GET parameter key for this field?
  if (!empty($_GET[$field_configuration['url_query_field']])) {
    // Yes. Get its value and check if it matches exactly one of the
    // $dropdown_options.
    $url_query_field_value = $_GET[$field_configuration['url_query_field']];

    if (array_key_exists($url_query_field_value, $dropdown_options)) {
      // Yes; the value in $_GET matches exactly one of our drop-down options,
      // so we will assume that's what we've been asked to select.
      $values_from_url = $url_query_field_value;

      // If we have a multi-value field, cast this string to an array.
      if ($always_get_multiple || $field_configuration['fapi_multiple_select']) {
        $values_from_url = (array) $values_from_url;
      }
    }
    else {
      // No; split the value up if it contains commas using Drupal's slightly-
      // nifty tag-splitter-y feature.
      $values_from_url = drupal_explode_tags($url_query_field_value);

      // Now, if this field doesn't allow multiple values, we should pick only
      // one of the values from the URL and use that as the default. There's
      // no really sane way to do this, but one approach is to assume that the
      // first option should take precedence (I know, I know; this is a silly
      // idea and will probably end in hot-fix tears...).
      if (!$field_configuration['fapi_multiple_select'] && !$always_get_multiple) {
        // Just take the first of the query values.
        $values_from_url = reset($values_from_url);
      }
    }
  }

  // Lastly, if the query field values is an array, sort the values
  // alphabetically.
  if (is_array($values_from_url)) {
    natcasesort($values_from_url);
  }

  return $values_from_url;
}

/**
 * Converts an array of Solr indexed values for a field into its aliases.
 *
 * For example, if we have a list of holiday types from a Solr search like:
 *
 * $solr_field_values = [
 *   'Adventure- Other' => 'Adventure- Other',
 *   'Islands &amp; Beaches- Other' => 'Islands &amp; Beaches- Other',
 *   'Luxury Holidays- Beach' => 'Luxury Holidays- Beach',
 *   'Short Breaks- Cities' => 'Short Breaks- Cities',
 *   'Luxury Holidays- City Breaks' => 'Luxury Holidays- City Breaks',
 * ];
 *
 * ... then this function will use fcl_fcuk_custom_facets_aliases_list() to
 * turn this list into:
 *
 * $solr_field_values = [
 *   // This was 'Adventure- Other'.
 *   'adventure' => 'Adventure',
 *
 *   // Note that 'Islands &amp; Beaches- Other' and 'Luxury Holidays- Beach'
 *   // are merged into a single 'Beach' entry.
 *   'beach' => 'Beach',
 *
 *   // Again, the two cities entries are merged into one here.
 *   'city_breaks' => 'City Breaks',
 * ];
 *
 * @param string $solr_field_name
 *   The field name in Solr which we are processing aliases for.
 *
 * @param array $solr_field_values
 *   A list of values returned from a Solr query.
 *
 * @return array
 *   A copy of $solr_field_values with any values which have an alias replaced
 *   with their aliases.
 */
function fcl_fcuk_ajax_search_forms_convert_solr_field_options_to_aliases($solr_field_name, array $solr_field_values) {
  // Get all holiday aliases.
  $aliases_list_reversed = fcl_fcuk_custom_facets_aliases_list_reverse_lookup('holidays');

  // Get the URL query field name.
  $url_query_field_name = fcl_fcuk_ajax_search_forms_get_url_query_name_from_solr_field($solr_field_name);

  // Initialise the list of aliases with a copy of the solr_field_values array.
  $solr_field_values_aliased = $solr_field_values;

  // Do we have a set of aliases for this field?
  if (array_key_exists($url_query_field_name, $aliases_list_reversed)) {
    // Yes. Process the aliases now.
    foreach ($solr_field_values as $solr_field_key => $solr_field_value) {
      // Do we have an alias for this option?
      if (array_key_exists($solr_field_value, $aliases_list_reversed[$url_query_field_name])) {
        // Yes, unset the old value and insert the alias if not already
        // present.
        unset($solr_field_values_aliased[$solr_field_key]);

        // Get the alias value.
        $alias_value = $aliases_list_reversed[$url_query_field_name][$solr_field_value]['value'];

        if (!array_key_exists($alias_value, $solr_field_values_aliased)) {
          // Not found; set it now.
          $solr_field_values_aliased[$alias_value] = $aliases_list_reversed[$url_query_field_name][$solr_field_value]['label'];
        }
      }
    }
  }

  return $solr_field_values_aliased;
}

/**
 * Updates the holidays search form via Ajax.
 *
 * This callback will return an array of valid options for the specified field.
 *
 * @see fcluk_procat_search_form_fields_ajax_callback()
 * @see fcluk_procat_search_menu()
 *
 * @param string $context
 *   The context, for example "holidays" or "flights".
 *
 * @param string $url_query_field
 *   The drop-down being updated; one of holiday type or star rating.
 */
function fcl_fcuk_ajax_search_forms_ajax_options_callback($context, $url_query_field) {
  // Get the facet Solr field name.
  $facet_solr_field = fcl_fcuk_ajax_search_forms_get_solr_field_name_from_url_query($url_query_field, $context);

  // Get the field config.
  $field_configuration = fcl_fcuk_ajax_search_forms_get_field_configuration($facet_solr_field);

  // Get the dropdown options.
  $dropdown_options = fcl_fcuk_ajax_search_forms_get_dropdown_options($field_configuration);

  // Get the selected option.
  $selected_option = fcl_fcuk_ajax_search_forms_get_url_query_values($field_configuration, $dropdown_options);

  // Turn the drop-down options into a chunk of HTML which we can use to
  // replace the contents of the requested drop-down.
  $dropdown_options_array = [];

  foreach ($dropdown_options as $dropdown_option_name => $dropdown_option_value) {
    $dropdown_option_render_array = [
      '#type' => 'html_tag',
      '#tag' => 'option',
      '#value' => $dropdown_option_value,
      '#attributes' => [
        'value' => $dropdown_option_name,
      ],
    ];

    // Is this the selected option?
    $this_is_selected = FALSE;
    if (is_array($selected_option)) {
      if (in_array($dropdown_option_name, $selected_option)) {
        // Yes - this should be marked "selected".
        $this_is_selected = TRUE;
      }
    }
    elseif ($selected_option == $dropdown_option_name) {
      $this_is_selected = TRUE;
    }

    if ($this_is_selected) {
      $dropdown_option_render_array['#attributes']['selected'] = 'selected';
    }

    $dropdown_options_array[] = $dropdown_option_render_array;
  }

  // Render the array.
  $dropdown_options_array_rendered = render($dropdown_options_array);

  // Get the element's ID, if provided.
  $element_id = '';
  if (!empty($_GET['id'])) {
    $element_id = '#' . check_plain($_GET['id']);
  }

  // Add some extra data in to the response.
  $response = [
    'options' => $dropdown_options_array_rendered,
    'selector' => 'select' . $element_id . '[name="' . $url_query_field . '"]',
  ];

  // Output the options as a chunk of Json.
  drupal_json_output(['status' => 0, 'data' => $response]);
}

/**
 * Implements hook_fcl_fcuk_ajax_search_forms_get_form_facets_alter().
 *
 * @param array $fields
 *   The available Apache Solr fields.
 *
 * @param string $context
 *   The context, for example "holidays" or "flights".
 */
function fcl_fcuk_ajax_search_forms_fcl_fcuk_ajax_search_forms_get_form_facets_alter(&$fields, $context) {
  // Tell select2 to only show results when two or more characters are entered
  // in the destinations drop-down.
  $fields['full_destination']['sm_itinerary_destination_full']['#attributes']['data-minimum-input-length'] = 2;

  // We also don't want to show the search box if there's only a small number
  // of solr_field_values, because this will drive customers mad trying to work
  // out what they can type in.
  $fields['full_destination']['sm_itinerary_destination_full']['#attributes']['data-minimum-results-for-search'] = 20;
}

/**
 * Converts an array of aliased Solr field values into its unaliased values.
 *
 * For example, if we have a list of holiday types from a search drop-down
 * like:
 *
 * $solr_field_values = [
 *   'adventure' => 'Adventure',
 *   'beach' => 'Beach',
 *   'city_breaks' => 'City Breaks',
 * ];
 *
 * ... then this function will use fcl_fcuk_custom_facets_aliases_list() to
 * turn this list into:
 *
 * $solr_field_values = [
 *   // This is mapped from 'Adventure'.
 *   'Adventure- Other' => 'Adventure- Other',
 *
 *   // Note that 'Beach' converts to both 'Islands &amp; Beaches- Other' and
 *   'Luxury Holidays- Beach'.
 *   'Islands &amp; Beaches- Other' => 'Islands &amp; Beaches- Other',
 *   'Luxury Holidays- Beach' => 'Luxury Holidays- Beach',
 *
 *   // Again, 'City Breaks' expands into two here.
 *   'Short Breaks- Cities' => 'Short Breaks- Cities',
 *   'Luxury Holidays- City Breaks' => 'Luxury Holidays- City Breaks',
 * ];
 *
 * @param string $solr_field_name
 *   The field name in Solr which we are processing aliases for.
 *
 * @param array $solr_field_values_aliased
 *   An array of aliased Solr field values.
 *
 * @return array
 *   A copy of $solr_field_values with any values which have an alias replaced
 *   with their aliases.
 */
function fcl_fcuk_ajax_search_forms_convert_aliases_to_solr_field_options($solr_field_name, array $solr_field_values_aliased) {
  // Get all holiday aliases.
  $aliases_list = fcl_fcuk_custom_facets_aliases_list('holidays');

  // Get the URL query field name.
  $url_query_field_name = fcl_fcuk_ajax_search_forms_get_url_query_name_from_solr_field($solr_field_name);

  // Initialise the list of aliases with a copy of the
  // solr_field_values_unaliased array.
  $solr_field_values_unaliased = $solr_field_values_aliased;

  // Do we have a set of aliases for this field?
  if (array_key_exists($url_query_field_name, $aliases_list)) {
    // Yes. Process the aliases now.
    foreach ($aliases_list[$url_query_field_name]['aliases'] as $alias_value => $alias_information) {
      // Does the key $alias_value exist in $solr_field_values_unaliased?
      if (array_key_exists($alias_value, $solr_field_values_unaliased)) {
        // Yep! Switch it out for its aliases.
        unset($solr_field_values_unaliased[$alias_value]);

        foreach ($alias_information['values'] as $unaliased_option) {
          $solr_field_values_unaliased[$unaliased_option] = $unaliased_option;
        }
      }
    }
  }

  return $solr_field_values_unaliased;
}

/**
 * Runs a Solr search to get a list of available options for a field.
 *
 * @param array $field_configuration
 *   A field configuration array from
 *   fcl_fcuk_ajax_search_forms_get_field_configuration().
 */
function fcl_fcuk_ajax_search_forms_get_dropdown_options($field_configuration) {
  // Get a static variaboo.
  $dropdown_options = &drupal_static(__FUNCTION__, []);

  // Build a cache ID.
  $cache_id = 'fcukdropdown:' . $field_configuration['url_query_field'] . '=*';

  // Initialise the Solr fq and fl arrays.
  $solr_field_query_array = [];
  $solr_field_list_array = [];

  // Set the Solr fl value for this search; this tells Solr we only want results
  // to contain the contents of this field.
  $solr_field_list_array[] = $field_configuration['solr_field'];

  // Does the field configuration tell us to get additional fields in this
  // search?
  if (!empty($field_configuration['solr_field_list_array'])) {
    foreach ($field_configuration['solr_field_list_array'] as $additional_field_to_get) {
      $solr_field_list_array[] = $additional_field_to_get;
    }
  }

  // Do we need to add any other filters specified in inherited_filters?
  if (!empty($field_configuration['inherited_filters'])) {
    foreach ($field_configuration['inherited_filters'] as $inherited_filter_solr_field_name) {
      $inherited_filter_field_configuration = fcl_fcuk_ajax_search_forms_get_field_configuration($inherited_filter_solr_field_name);

      // Get any values for this field from the URL; these could be aliased,
      // so we need to unalias them.
      $inherited_filter_values_aliased = (array) fcl_fcuk_ajax_search_forms_get_url_query_values($inherited_filter_field_configuration, [], TRUE);

      // Convert the values from raw (i.e. with underscores) to normal (i.e.
      // with spaces).
      foreach ($inherited_filter_values_aliased as $key => $inherited_filter_value_aliased) {
        $inherited_filter_values_aliased[$key] = fcl_fcuk_ajax_search_forms_url_to_string($inherited_filter_value_aliased);
      }

      // Unalias the values.
      $inherited_filter_values_unaliased = fcl_fcuk_ajax_search_forms_convert_aliases_to_solr_field_options($inherited_filter_solr_field_name, $inherited_filter_values_aliased);

      // Remove wildcards.
      foreach ($inherited_filter_values_unaliased as $key => $inherited_filter_value_unaliased) {
        if ($inherited_filter_value_unaliased == '*') {
          unset($inherited_filter_values_unaliased[$key]);
        }
      }

      // Add the inherited filter, if it isn't empty.
      if (!empty($inherited_filter_values_unaliased)) {
        $solr_field_query_array[] = $inherited_filter_solr_field_name . ':'
          . fcl_uk_utilities_array_to_solr_query($inherited_filter_values_unaliased);
      }

      // Are we filtering on this inherited filter? If it's just an asterisk,
      // then we aren't.
      $inherited_filter_cache_id_string = implode('+', fcl_fcuk_ajax_search_forms_string_to_cache($inherited_filter_values_aliased));

      if (!empty($inherited_filter_cache_id_string) && !($inherited_filter_cache_id_string == '*')) {
        // Append the field information to the cache ID, so we can correctly
        // separate the query results. We append the field's URL query string,
        // and then the list of query parameters. For example, if the field
        // name in the URL is 'destination', and the list of destinations is
        // 'kenya,africa,new zealand', then the cache ID will be appended with:
        // :destination=kenya+africa+newzealand.
        $cache_id .= ':' . $inherited_filter_field_configuration['url_query_field']
          . '=' . $inherited_filter_cache_id_string;
      }
    }
  }

  // Do we have this query in static?
  if (!empty($dropdown_options[$cache_id])) {
    return $dropdown_options[$cache_id];
  }

  // Do we have this query in the cache?
  if (fcl_fcuk_ajax_search_forms_cache_enabled()) {
    $cache_result = cache_get($cache_id);

    if (!empty($cache_result->data) && is_array($cache_result->data)) {
      // Yes, it's in the cache. Add it to the static variable and return.
      $dropdown_options[$cache_id] = $cache_result->data;

      return $dropdown_options[$cache_id];
    }
  }

  // Not in the cache; initialise the results array and build a Solr query.
  $dropdown_options[$cache_id] = [];

  // Get the Solr connection info.
  $solr = apachesolr_get_solr(FCLUK_MERCURY_ENV);

  // Add the entity type to the field query.
  $solr_field_query_array[] = 'entity_type:' . $field_configuration['entity_type'];

  // Merge in any other field queries.
  $solr_field_query_array = array_merge($solr_field_query_array, $field_configuration['solr_field_query_array']);

  // Build the Solr query.
  $params = [
    'rows' => FCL_MERCURY_SEARCH_LIMIT,
  ];

  // Do we have a sort?
  if (array_key_exists('solr_sort', $field_configuration)) {
    $params['sort'] = $field_configuration['solr_field'] . ' asc';
  }

  if (!empty($solr_field_query_array)) {
    $params['fq'] = [implode(' AND ', $solr_field_query_array)];
  }

  if (!empty($solr_field_list_array)) {
    $params['fl'] = implode(',', $solr_field_list_array);
  }

  // Implement a hook_fcl_fcuk_ajax_search_forms_get_dropdown_options_query_alter.
  // We make the arrays which built the params available to the alter so that
  // any alter functions can re-create the params array. For example, we
  // can use this to adjust the search, for example so we can search on
  // multiple destination fields.
  $information_for_alter_hooks = [
    'solr_field_query_array' => $solr_field_query_array,
    'solr_field_list_array' => $solr_field_list_array,
    'field_configuration' => $field_configuration,
  ];

  // @todo: should we do this with a specific query alter function instead?
  drupal_alter('fcl_fcuk_ajax_search_forms_get_dropdown_options_query', $params, $information_for_alter_hooks);

  // Run the query.
  $response = $solr->search('', $params);

  // Did the query run successfully?
  if (!empty($response->status_message)
    && ($response->status_message === 'OK')
  ) {
    if ($response->response->numFound >= 1) {
      // Build the results array.
      foreach ($response->response->docs as $search_result) {
        $dropdown_options[$cache_id][] = $search_result;
      }
    }
  }

  // Implement a hook_fcl_fcuk_ajax_search_forms_dropdown_options_alter.
  drupal_alter('fcl_fcuk_ajax_search_forms_dropdown_options', $dropdown_options[$cache_id], $field_configuration);

  // If the field configuration specifies any processor functions, for example
  // to convert the results objects into strings, run them now. We use these,
  // for example, in the destinations field so we can add the destination's
  // hierarchy into the destination title. This function is also where we
  // flatten the result objects into a Drupal form API-friendly string so
  // the array of options can be used in a drop-down select.
  if (!empty($field_configuration['process_functions'])) {
    // Is it an array? If not, cast to an array now.
    if (!is_array($field_configuration['process_functions'])) {
      $field_configuration['process_functions'] = (array) $field_configuration['process_functions'];
    }

    foreach ($field_configuration['process_functions'] as $process_function) {
      if (function_exists($process_function)) {
        $process_function($dropdown_options[$cache_id], $field_configuration);

        // Implement a hook_<process function>_alter.
        drupal_alter($process_function, $dropdown_options[$cache_id], $field_configuration);
      }
    }
  }

  // If requested, add in the "All results" option and "No results" options.
  fcl_fcuk_ajax_search_forms_dropdown_options_add_all_option($dropdown_options[$cache_id], $field_configuration);
  fcl_fcuk_ajax_search_forms_dropdown_options_add_no_results_option($dropdown_options[$cache_id], $field_configuration);

  // Write it to the cache. Set an expiry time 15 minutes in the future.
  if (fcl_fcuk_ajax_search_forms_cache_enabled()) {
    cache_set($cache_id, $dropdown_options[$cache_id], 'cache',
      REQUEST_TIME + variable_get('fcl_fcuk_ajax_search_forms_cache_lifetime', FCL_FCUK_AJAX_SEARCH_FORMS_CACHE_LIFETIME_DEFAULT));
  }

  // Return the data.
  return $dropdown_options[$cache_id];
}

/**
 * Implements
 * hook_fcl_fcuk_ajax_search_forms_get_dropdown_options_query_alter().
 *
 * Allow searches by destination to look at the destination hierarchy field,
 * too.
 *
 * Also, allow holiday type searches for "Beach" to search for "beach", too.
 *
 * @param array $params
 *   The parameters array which is just about to be passed to the Solr server.
 *
 * @param array $information_for_alter_hooks
 *   An array containing the following information:
 *
 *   [
 *     'solr_field_query_array' => [
 *       'entity_type:mercury_holiday AND
 *       bs_mercury_holiday_fcuk_publish_status:1',
 *     ],
 *     'solr_field_list_array' => [
 *       'ss_mercury_holiday_segments_destinations_hierarchy',
 *       'some_other_field',
 *     ],
 *     'field_configuration' => <a field configuration array>,
 *   ]
 */
function fcl_fcuk_ajax_search_forms_fcl_fcuk_ajax_search_forms_get_dropdown_options_query_alter(&$params, $information_for_alter_hooks) {
  $field_configuration = $information_for_alter_hooks['field_configuration'];
  $solr_field_query_array = $information_for_alter_hooks['solr_field_query_array'];
  $solr_field_list_array = $information_for_alter_hooks['solr_field_list_array'];

  foreach ($solr_field_query_array as &$solr_field_query_parameter) {
    // Allow searching on
    // ss_mercury_holiday_segments_destinations_hierarchy as well as
    // ss_mercury_holiday_key_destination; search through the fq array.
    if (substr($solr_field_query_parameter, 0, strlen('ss_mercury_holiday_key_destination:')) == 'ss_mercury_holiday_key_destination:') {
      // Split the query into a key/value pair.
      $query_key_value_pair = explode(':', $solr_field_query_parameter, 2);

      // Get the values in a convenience variaboo.
      $values = explode(' OR ', $query_key_value_pair[1]);

      // Create the array for the new parameter.
      $new_solr_field_query_parameter_array = [];

      foreach ($values as $value) {
        $value = trim($value, ' "()');

        // Create a new parameter. Because the destination hierarchy field
        // stores the hierarchy as "Destination;Parent;Grandparent;...", we
        // need to search for "Destination;*", "*;Destination;*" or
        // "*;Destination".
        $new_solr_field_query_parameter_array[] = $value;
        $new_solr_field_query_parameter_array[] = $value . ';*';
        $new_solr_field_query_parameter_array[] = '*;' . $value;
        $new_solr_field_query_parameter_array[] = '*;' . $value . ';*';
      }

      $solr_field_query_parameter = '(' . $solr_field_query_parameter . ' OR ss_mercury_holiday_segments_destinations_hierarchy:("' . implode('" OR "', $new_solr_field_query_parameter_array) . '"))';
    }

    // Search holiday types in various cases.
    if (substr($solr_field_query_parameter, 0, strlen('sm_mercury_holiday_type:')) == 'sm_mercury_holiday_type:') {
      // Split the query into a key/value pair.
      $query_key_value_pair = explode(':', $solr_field_query_parameter, 2);

      // Get the value in a convenience variaboo.
      $value = trim($query_key_value_pair[1], ' "()');

      // Create a new search parameter where we also try to match on upper-
      // case first letter, all lower case, and the original value, whatever
      // capitalisation it may have.
      $solr_field_query_parameter = 'sm_mercury_holiday_type:("' . $value . '" OR "' . ucwords($value) . '" OR "' . strtolower($value) . '" OR "' . strtoupper($value) . '")';
    }
  }

  // Re-add the query array back into the query parameters.
  $params['fq'] = [implode(' AND ', $solr_field_query_array)];
}

/**
 * Sorts the dropdown options using a natural case-insensitive sort.
 *
 * @param array $dropdown_options
 *   The list of dropdown options being built.
 *
 * @param array $field_configuration
 *   The field configuration array, from
 *   fcl_fcuk_ajax_search_forms_get_field_configuration().
 */
function fcl_fcuk_ajax_search_forms_dropdown_options_natcasesort(&$dropdown_options, $field_configuration) {
  natcasesort($dropdown_options);
}

/**
 * Sorts the dropdown options using a backwards natural case-insensitive sort.
 *
 * @param array $dropdown_options
 *   The list of dropdown options being built.
 *
 * @param array $field_configuration
 *   The field configuration array, from
 *   fcl_fcuk_ajax_search_forms_get_field_configuration().
 */
function fcl_fcuk_ajax_search_forms_dropdown_options_natcasesort_reversed(&$dropdown_options, $field_configuration) {
  natcasesort($dropdown_options);
  $dropdown_options = array_reverse($dropdown_options, TRUE);
}

/**
 * Adds an "All results" option to a drop-down if the field config specifies it.
 *
 * Note that, if there is only one entry in the list, this function will not
 * add an "All results" option, because there would be no point.
 *
 * @param array $dropdown_options
 *   The list of dropdown options being built.
 *
 * @param array $field_configuration
 *   The field configuration array, from
 *   fcl_fcuk_ajax_search_forms_get_field_configuration().
 */
function fcl_fcuk_ajax_search_forms_dropdown_options_add_all_option(&$dropdown_options, $field_configuration) {
  // Do we have an all_results option?
  if ((count($dropdown_options) > 1) && !empty($field_configuration['all_results'])) {
    // Yes; do we add the default all_results text?

    $dropdown_options = [NULL => _fcl_fcuk_ajax_search_forms_dropdown_options_get_all_option($field_configuration)] + $dropdown_options;
  }
}

/**
 * Works out the "All results" option text for a field.
 *
 * This function is also used to get the placeholder text used on a field, e.g.
 * by Select2.
 *
 * @param array $field_configuration
 *   The field configuration array, from
 *   fcl_fcuk_ajax_search_forms_get_field_configuration().
 *
 * @return string
 *   The "All results" placeholder text.
 */
function _fcl_fcuk_ajax_search_forms_dropdown_options_get_all_option($field_configuration) {
  if ($field_configuration['all_results'] === TRUE) {
    // Yes.
    $all_option_text = t('-- All --');
  }
  else {
    // No - this should be a custom string of text which has already been
    // passed through t().
    $all_option_text = (string) $field_configuration['all_results'];
  }

  return $all_option_text;
}

/**
 * If no results have been found, this will add a "No results found" option.
 **
 *
 * @param array $dropdown_options
 *   The list of dropdown options being built.
 *
 * @param array $field_configuration
 *   The field configuration array, from
 *   fcl_fcuk_ajax_search_forms_get_field_configuration().
 */
function fcl_fcuk_ajax_search_forms_dropdown_options_add_no_results_option(&$dropdown_options, $field_configuration) {
  // Do we have a no_results option?
  if (empty($dropdown_options) && !empty($field_configuration['no_results'])) {
    // Yes; do we add the default no_results text?
    if ($field_configuration['no_results'] === TRUE) {
      // Yes.
      $no_results_text = t('--- No results found ---');
    }
    else {
      // No - this should be a custom string of text which has already been
      // passed through t().
      $no_results_text = (string) $field_configuration['no_results'];
    }

    $dropdown_options = [NULL => $no_results_text];
  }
}

/**
 * Example function: add contents of a search result into drop-down options.
 *
 * This function will create an entry in the drop-down options array
 * whose key is the value of the search result primary field result.
 *
 * Search result primary field result is the primary field which we
 * are asking Solr for. For example, if we're building a list of
 * destinations, it will be the field which contains the destination.
 *
 * For example, $search_result_primary_field_result is "New Malden",
 * and $search_result_record is:
 *
 * [
 *   'Monkey',
 *   'Trevor',
 *   'New Malden',
 *   'Bananarama',
 * ]
 *
 * ... then we will add the following array key to
 * $dropdown_options_array:
 *
 * $dropdown_options_array['New Malden'] = [
 *   'Monkey',
 *   'Trevor',
 *   'New Malden',
 *   'Bananarama',
 * ];
 *
 * In some cases, the search result primary field result field may be
 * a multi-value field; in these situations, we loop through each
 * value in the field and create an array value for each keyed on
 * that particular value in the multi-value field.
 *
 * As an example, if $search_result_primary_field_result is now:
 *
 * [
 *   'New Malden',
 *   'Button Moon',
 *   'Way Out West',
 * ]
 *
 * ... and and $search_result_record has become:
 *
 * [
 *   'Monkey',
 *   'Trevor',
 *   [
 *     'New Malden',
 *     'Button Moon',
 *     'Way Out West',
 *   ],
 *   'Bananarama',
 * ]
 *
 * ... then we will do the following with $dropdown_options_array:
 *
 * $dropdown_options_array['New Malden'] = [
 *   'Monkey',
 *   'Trevor',
 *   [
 *     'New Malden',
 *     'Button Moon',
 *     'Way Out West',
 *   ],
 *   'Bananarama',
 * ];
 *
 * $dropdown_options_array['Button Moon'] = [
 *   'Monkey',
 *   'Trevor',
 *   [
 *     'New Malden',
 *     'Button Moon',
 *     'Way Out West',
 *   ],
 *   'Bananarama',
 * ];
 *
 * $dropdown_options_array['Way Out West'] = [
 *   'Monkey',
 *   'Trevor',
 *   [
 *     'New Malden',
 *     'Button Moon',
 *     'Way Out West',
 *   ],
 *   'Bananarama',
 * ];
 *
 * @param array $dropdown_options_array
 *   The array of drop-down options which we are assembling.
 *
 * @param array|string $search_result_primary_field_result
 *   The primary field from the search result which we will use as
 *   the key for the entry in the results array which will contain
 *   the whole search result record.
 *
 * @param              $search_result_record
 */
function _fcl_fcuk_ajax_search_forms_process_solr_result(&$dropdown_options_array, $search_result_primary_field_result, $search_result_record) {
//  // Check if the result is an array or object or just a single value.
//  if (is_array($search_result_primary_field_result) || is_object($search_result_primary_field_result)) {
//    // Yes; loop through and create multiple search results.
//    foreach ($search_result_primary_field_result as $this_result_subvalue) {
//      _fcl_fcuk_ajax_search_forms_process_solr_result($dropdown_options_array, $this_result_subvalue, $search_result_record);
//    }
//  }
//  else {
//    // No; it's just a single result.
//    $dropdown_options_array[$search_result_primary_field_result] = $search_result_record;
//  }
}

/**
 * Example: implements hook_fcl_fcuk_ajax_search_forms_dropdown_options_alter().
 *
 * @param array $dropdown_options
 *   The list of dropdown options being built.
 *
 * @param array $field_configuration
 *   The field configuration array, from
 *   fcl_fcuk_ajax_search_forms_get_field_configuration().
 */
function fcl_fcuk_ajax_search_forms_fcl_fcuk_ajax_search_forms_dropdown_options_alter(&$dropdown_options, $field_configuration) {
}

/**
 * Processor to build a list of destinations from raw Solr data.
 *
 * @param array $dropdown_options
 *   The list of dropdown options being built.
 *
 * @param array $field_configuration
 *   The field configuration array, from
 *   fcl_fcuk_ajax_search_forms_get_field_configuration().
 */
function fcl_fcuk_ajax_search_forms_dropdown_options_process_destinations(&$dropdown_options, $field_configuration) {
  // Start by getting all mercury_destinations.
  $mercury_destinations_by_entity_id = fcl_fcuk_ajax_search_forms_get_mercury_holiday_destinations();

  // Re-write the list of drop-down options to just get a list of the
  // destination entity IDs from the
  // im_mercury_holiday_segments_destinations_tids field; we then use that
  // to build the list of destinations.
  $dropdown_options_entity_ids_list = [];
  foreach ($dropdown_options as $dropdown_option) {
    foreach ($dropdown_option->im_mercury_holiday_segments_destinations_tids as $destination_entity_id) {
      // If the destination entity ID is in the list of Mercury destinations,
      // add it to the list.
      if (!empty($mercury_destinations_by_entity_id[$destination_entity_id])) {
        $dropdown_options_entity_ids_list[$destination_entity_id] = $mercury_destinations_by_entity_id[$destination_entity_id];
      }
    }
  }

  // Ignore the following destination entity IDs.
  // @todo: allow this to be altered and/or get from a variable_get().
  $destinations_to_ignore = [
    6 => 'World',
  ];

  // Now we have a list of all the destination entity IDs which can be found
  // by running a holidays search, we need to get the list of each destination's
  // parents in a text string array so we can implode them quickly.
  $dropdown_options_processed = [];
  foreach ($dropdown_options_entity_ids_list as $destination_entity_id => $destination_entity) {
    if (!empty($destination_entity->im_mercury_destination_hierarchy)) {
      // Get the list of destinations from im_mercury_destination_hierarchy.
      // This array ends with "World" and drills down to (and includes) the
      // destination's name. So, we start by filtering out any destinations
      // to be ignored.
      $destination_hierarchy = $destination_entity->im_mercury_destination_hierarchy;

      // Get the destination's name.
      $destination_name = $mercury_destinations_by_entity_id[$destination_entity_id]->ss_mercury_destination_name;

      // Is the destination in the ignore list?
      if (array_key_exists($destination_entity_id, $destinations_to_ignore)) {
        continue;
      }

      // Does the destination have any parents which we should filter out?
      foreach ($destinations_to_ignore as $destination_to_ignore_entity_id => $destination_to_ignore_name) {
        $destination_to_ignore_in_hierarchy_search_result = array_search($destination_to_ignore_entity_id, $destination_hierarchy);

        if ($destination_to_ignore_in_hierarchy_search_result !== FALSE) {
          unset($destination_hierarchy[$destination_to_ignore_in_hierarchy_search_result]);
        }
      }

      // Get the parents' names for the destination, if we have any.
      $destination_parents_entity_names_array = [];
      if (count($destination_hierarchy) > 1) {
        $destination_parents_entity_ids_array = array_slice($destination_hierarchy, 1);

        foreach ($destination_parents_entity_ids_array as $destination_parent_entity_id) {
          $destination_parents_entity_names_array[] = $mercury_destinations_by_entity_id[$destination_parent_entity_id]->ss_mercury_destination_name;
        }

        $destination_parents_entity_names = ' (' . implode(', ', $destination_parents_entity_names_array) . ')';
      }
      else {
        $destination_parents_entity_names = ' ' . t('(all destinations)');
      }

      $destination_dropdown_option = trim($destination_name . $destination_parents_entity_names);

      // Get the destination and parents' names for the Solr search, and
      // convert them to URL-friendly strings.
      $destination_query_entity_names_array = array_merge([$destination_name], $destination_parents_entity_names_array);

      foreach ($destination_query_entity_names_array as &$destination_query_entity_name) {
        $destination_query_entity_name = str_replace(' ', '_', check_plain($destination_query_entity_name));
      }

      $destination_dropdown_value = trim(implode(',', $destination_query_entity_names_array));

      // If neither query string or title are empty, add the destination info
      // into the dropdowns list.
      if (!empty($destination_dropdown_value) && !empty($destination_dropdown_option)) {
        $dropdown_options_processed[$destination_dropdown_value] = $destination_dropdown_option;
      }
    }
  }

  // Overwrite the $dropdown_options array with the contents of
  // $dropdown_options_processed.
  $dropdown_options = $dropdown_options_processed;
}

/**
 * Processor to build a list of holiday types from raw Solr data.
 *
 * This function simply takes the array keys of the holiday types found
 * in Solr and uses them as the array value.
 *
 * @param array $dropdown_options
 *   The list of dropdown options being built.
 *
 * @param array $field_configuration
 *   The field configuration array, from
 *   fcl_fcuk_ajax_search_forms_get_field_configuration().
 */
function fcl_fcuk_ajax_search_forms_dropdown_options_process_types(&$dropdown_options, $field_configuration) {
  // Create a new array to build our holiday types in.
  $dropdown_options_processed = [];

  foreach ($dropdown_options as $dropdown_option) {
    // The holiday type field is a multi-value field, so loop through any
    // values in the field.
    foreach ($dropdown_option->{$field_configuration['solr_field']} as $holiday_type_name) {
      if (!array_key_exists($holiday_type_name, $dropdown_options_processed)) {
        $dropdown_options_processed[$holiday_type_name] = $holiday_type_name;
      }
    }
  }

  $dropdown_options = $dropdown_options_processed;
}

/**
 * Processor to build a list of holiday star ratings from raw Solr data.
 *
 * Holiday ratings are an array of one or more star ratings. For example,
 * $dropdown_options might be:
 *
 * [
 *   '4' => [
 *     'fm_mercury_holiday_hotel_star_rating' => [
 *       '4.5',
 *       '4',
 *       '5',
 *     ],
 *   '5' => [
 *     'fm_mercury_holiday_hotel_star_rating' => [
 *       '5',
 *     ],
 *   '3.5' => [
 *     'fm_mercury_holiday_hotel_star_rating' => [
 *       '4',
 *       '3.5',
 *       '3',
 *       '2',
 *     ],
 *   ],
 * ]
 *
 * This means that, even though we only technically have entries in this list
 * for 3.5, 4 and 5 star holidays, a Solr search for holidays with a
 * fm_mercury_holiday_hotel_star_rating of 2 would turn up results (because
 * one of the arrays of values in the example above contains a 2).
 *
 * So we need to extract all the possible star ratings.
 *
 * @param array $dropdown_options
 *   The list of dropdown options being built.
 *
 * @param array $field_configuration
 *   The field configuration array, from
 *   fcl_fcuk_ajax_search_forms_get_field_configuration().
 */
function fcl_fcuk_ajax_search_forms_dropdown_options_process_ratings(&$dropdown_options, $field_configuration) {
  // Create a new array to build our holiday types in.
  $dropdown_options_processed = [];

  foreach ($dropdown_options as $dropdown_option) {
    // Prevent empty rating results.
    if (empty($dropdown_option->{$field_configuration['solr_field']})) {
      continue;
    }

    // Get all ratings for each $dropdown_option.
    foreach ($dropdown_option->{$field_configuration['solr_field']} as $dropdown_option_rating) {
      // If the star rating is below 3, we hide it - seems that nobody wants
      // to be seen to be voluntarily looking for a cheapo holiday.
      if (intval($dropdown_option_rating) < 3) {
        continue;
      }

      // Case $dropdown_option_rating to a string to prevent odd
      // "Warning: array_key_exists(): The first argument should be either a
      // string or an integer" whingeing.
      $dropdown_option_rating_value = (string) number_format($dropdown_option_rating, 1);
      $dropdown_option_rating = (string) $dropdown_option_rating;

      if (!array_key_exists($dropdown_option_rating_value, $dropdown_options_processed)) {
        $dropdown_options_processed[$dropdown_option_rating_value] = t('@rating stars', ['@rating' => $dropdown_option_rating]);
      }
    }
  }

  $dropdown_options = $dropdown_options_processed;
}

/**
 * Gets a list of Mercury Holiday destinations.
 *
 * @return array
 *   An array of Mercury holiday destinations, where the array key is the
 *   Mercury destination's entity ID, and the value is the destination's name
 *   in Mercury.
 */
function fcl_fcuk_ajax_search_forms_get_mercury_holiday_destinations() {
  // Set a cache ID.
  $cache_id = 'fclfcukstaticformsmercuryholidaydestinations';

  // Get the static array to hold these destinations.
  $destinations = &drupal_static($cache_id);

  // Do we have destinations?
  if (!empty($destinations)) {
    return $destinations;
  }

  // Nothing in static; check the cache.
  if (fcl_fcuk_ajax_search_forms_cache_enabled()) {
    $cache_result = cache_get($cache_id);

    if (!empty($cache_result->data)) {
      // Cache hit. Store in the static array and return.
      $destinations = $cache_result->data;
      return $destinations;
    }
  }

  // Nothing in the cache; query Solr. Get the Solr connection info.
  $solr = apachesolr_get_solr(FCLUK_MERCURY_ENV);

  // Build the Solr query.
  $params = [
    'fq' => ['entity_type:mercury_destination'],
    'fl' => '*',
    'rows' => (FCL_MERCURY_SEARCH_LIMIT * 100),
    'sort' => 'ss_mercury_destination_name asc',
  ];

  // Run the query.
  $response = $solr->search('', $params);

  // Did the query run successfully?
  if (!empty($response->status_message)
    && ($response->status_message === 'OK')
  ) {
    if ($response->response->numFound >= 1) {
      // Build the results array, where keys and values are the same.
      foreach ($response->response->docs as $search_result) {
        // Remove empty and [DEFAULT_DEPARTURE] entries.
        if (empty($search_result->is_mercury_destination_id)
          || empty($search_result->ss_mercury_destination_name)
        ) {
          continue;
        }

        if (($search_result->is_mercury_destination_id == '[DEFAULT_DEPARTURE]')
          || ($search_result->ss_mercury_destination_name == '[DEFAULT_DEPARTURE]')
        ) {
          continue;
        }

        $destinations[$search_result->is_mercury_destination_id] = $search_result;
      }
    }
  }

  // Implement a hook_fcl_fcuk_ajax_search_forms_mercury_holiday_destinations_alter.
  drupal_alter('fcl_fcuk_ajax_search_forms_mercury_holiday_destinations', $destinations);

  // Cache the data. @todo: make expiry configuraboo.
  if (fcl_fcuk_ajax_search_forms_cache_enabled()) {
    cache_set($cache_id, $destinations, 'cache', (REQUEST_TIME + FCL_FCUK_AJAX_SEARCH_FORMS_CACHE_LIFETIME_DEFAULT));
  }

  // Return the data.
  return $destinations;
}

/**
 * Add in destinations' parents to the list of destination options.
 *
 * We use this function to expand the list of available destinations to
 * include the destinations' parents.
 *
 * For example, sunny New Malden might have parents Surrey, England, UK, and
 * Button Moon.
 *
 * If I search for holidays in England, then the results will also include
 * holidays in New Malden; as a customer, I might not know about the cultural
 * delights of New Malden - for example the awesome Sakura restaurant - so I
 * might search using a more-broad term such as "England".
 *
 * But, because we may not have any holidays whose location is listed as
 * "England", then the list of available destinations that we build up won't
 * necessarily include England, since we've only assembled a list of the
 * actual locations of holidays, and not their parent locations.
 *
 * So, this function expands the list of destinations to include not only
 * our holidays' final destinations, but also their parents.
 *
 * If it helps to visualise this, here's the data we might start with:
 *
 * - New Malden
 * -- Parents:
 * --- 1. Surrey (i.e. closest parent)
 * --- 2. England (grandparent)
 * --- 3. UK (great-grandparent)
 * --- 4. Button Moon (great-great... you get the idea).
 *
 * Once we have processed this data, we will be left with:
 *
 * - New Malden (unchanged)
 * -- Parents:
 * --- 1. Surrey (i.e. closest parent)
 * --- 2. England (grandparent)
 * --- 3. UK (great-grandparent)
 * --- 4. Button Moon (great-great... you get the idea).
 * - Surrey
 * -- Parents:
 * --- 1. England
 * --- 2. UK
 * --- 3. Button Moon
 * - England
 * -- Parents:
 * --- 1: UK
 * --- 2: Button Moon
 * - UK
 * -- Parents:
 * --- 1: Button Moon
 * - Button Moon
 * -- Parents: <none>
 *
 * @param array $destination_options
 *   An array of the destination options for the destinations drop-down, where
 *   the array keys are the URL query parameter-formatted destination string,
 *   e.g. New_Malden, and the values are the fields returned from the Solr
 *   query, before they are flattened down into a single text string.
 *   This response is an object and must contain an array named
 *   im_mercury_holiday_destinations_tids, which contains a list of parents,
 *   ordered from nearest (parent) to furthest/highest (i.e. World).
 *
 * @param array $mercury_destinations_by_entity_id
 *   An array of destinations from Mercury, where the key is the destination
 *   entity ID, and the value is the destination's name.
 */
function fcl_fcuk_ajax_search_forms_mercury_holiday_destinations_add_parents(&$destination_options, $mercury_destinations_by_entity_id) {
  // Loop through each destination option.
  foreach ($destination_options as $destination_friendly_name => &$destination_option) {
    // Confirm it has a ->im_mercury_holiday_destinations_tids value.
    if (!empty($destination_option->im_mercury_holiday_destinations_tids)) {
      // Recursively add parents in to the list.
      _fcl_fcuk_ajax_search_forms_mercury_holiday_destinations_add_parents($destination_options, $mercury_destinations_by_entity_id, $destination_option->im_mercury_holiday_destinations_tids);
    }
  }
}

/**
 * Expands a list of destinations to include parent destinations, too.
 *
 * This function is called from within
 * fcl_fcuk_ajax_search_forms_mercury_holiday_destinations_add_parents().
 *
 * @see fcl_fcuk_ajax_search_forms_mercury_holiday_destinations_add_parents().
 *
 * @param array $destination_options
 *   An array of the destination options for the destinations drop-down, where
 *   the array keys are the URL query parameter-formatted destination name,
 *   e.g. New_Malden, and the values are the fields returned from the Solr
 *   query, before they are flattened down into a single text string.
 *   This response is an object and must contain an array named
 *   im_mercury_holiday_destinations_tids, which contains a list of parents,
 *   ordered from nearest (parent) to furthest/highest (i.e. World).
 *
 * @param array $mercury_destinations_by_entity_id
 *   An array of destinations from Mercury, where the key is the destination
 *   entity ID, and the value is the destination's name.
 *
 * @param array $destination_parent_entity_ids
 *   An array of the destination entity IDs of a given destination's parents,
 *   which we are going to extract into $destination_options as destinations
 *   in their own right.
 */
function _fcl_fcuk_ajax_search_forms_mercury_holiday_destinations_add_parents(array &$destination_options = [], array $mercury_destinations_by_entity_id = [], array $destination_parent_entity_ids = []) {
  if (!empty($destination_parent_entity_ids)) {
    // Pull the closest destination parent from $destination_parent_entity_ids and
    // add it in to the list of available destinations.
    $destination_entity_id_for_this_level = array_shift($destination_parent_entity_ids);

    // Does the destination's tid exist in the list of destinations?
    if (array_key_exists($destination_entity_id_for_this_level, $mercury_destinations_by_entity_id)) {
      // Get the destination's name.
      $destination_name = $mercury_destinations_by_entity_id[$destination_entity_id_for_this_level];

      // Check if the parent destination already exists - there's a reasonable
      // chance we have already added it to this list from another destination.
      if (!array_key_exists($destination_name, $destination_options)) {
        // We haven't processed it yet; create a destination option object in
        // the same format as the $destination_options results from the Solr
        // index.
        $destination_option = (object) [
          'ss_mercury_holiday_key_destination' => $mercury_destinations_by_entity_id[$destination_entity_id_for_this_level],
          'im_mercury_holiday_destinations_tids' => $destination_parent_entity_ids,
        ];

        // Add the new destination option back into the list of destination
        // options.
        $destination_options[$destination_name] = $destination_option;
      }
    }

    // Do we have more parents to process?
    if (!empty($destination_parent_entity_ids)) {
      _fcl_fcuk_ajax_search_forms_mercury_holiday_destinations_add_parents($destination_options, $mercury_destinations_by_entity_id, $destination_parent_entity_ids);
    }
  }
}

/**
 * Convert a string to be used in a URL query string for Solr.
 *
 * Currently, this function just converts spaces to underscores.
 *
 * @todo: should this also remove other characters? Use Path Auto maybe?
 *
 * @param string $string
 *   The string to be converted.
 *
 * @return string
 *   The string converted for use in a Solr query's URL.
 */
function fcl_fcuk_ajax_search_forms_convert_to_url($string = '') {
  $string = str_replace(' ', '_', $string);

  // Implement a hook_fcl_fcuk_ajax_search_forms_convert_to_url_alter.
  drupal_alter('fcl_fcuk_ajax_search_forms_convert_to_url', $string);

  return $string;
}

/**
 * Convert a string from a URL-formatted string to normal (with-spaces).
 *
 * Currently, this function just converts underscores to spaces.
 *
 * @todo: should this also remove other characters? Use Path Auto maybe?
 *
 * @param string $string
 *   The string to be converted.
 *
 * @return string
 *   The string converted for use in a Solr query's URL.
 */
function fcl_fcuk_ajax_search_forms_url_to_string($string = '') {
  $string = str_replace('_', ' ', $string);

  // Implement a hook_fcl_fcuk_ajax_search_forms_url_to_string_alter.
  drupal_alter('fcl_fcuk_ajax_search_forms_url_to_string', $string);

  return $string;
}

/**
 * Convert one or more values into cache-friendly strings.
 *
 * This function will remove non-alphanumerics but not convert strings to
 * lowercase because Solr uses some case-sensitive matching. :(
 *
 * @param mixed $value_or_values
 *   A string to be cleaned, or an array of values to be cleaned.
 *
 * @return mixed
 *   A cleaned-up string or array.
 */
function fcl_fcuk_ajax_search_forms_string_to_cache($value_or_values) {
  // Keep a track of whether we need to convert value back to being a
  // string.
  $is_string = is_string($value_or_values);

  if ($is_string) {
    $value_or_values = (array) $value_or_values;
  }

  foreach ($value_or_values as &$value) {
    // Remove non-alphanumerics.
    $value = preg_replace("/[^A-Za-z0-9]/", '', $value);
  }

  if ($is_string) {
    $value_or_values = reset($value_or_values);
  }

  return $value_or_values;
}

/**
 * Check whether the module's cache is enabled.
 *
 * This function checks for a variable named
 * "fcl_fcuk_ajax_search_forms_cache_enabled". If not found, the module falls
 * back to checking the global "cache" variable which determines whether
 * page data is cached.
 *
 * If for any reason that variable isn't available, the module falls back to
 * use the value of FCL_FCUK_AJAX_SEARCH_FORMS_CACHE_ENABLED_DEFAULT.
 *
 * @return bool TRUE if the module should cache data, or FALSE if not.
 */
function fcl_fcuk_ajax_search_forms_cache_enabled() {
  return variable_get('fcl_fcuk_ajax_search_forms_cache_enabled',
    variable_get('cache', FCL_FCUK_AJAX_SEARCH_FORMS_CACHE_ENABLED_DEFAULT));
}
