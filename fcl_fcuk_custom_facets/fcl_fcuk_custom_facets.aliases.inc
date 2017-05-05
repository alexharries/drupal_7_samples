<?php
/**
 * @file
 * Aliases for facet values on apache solr search.
 */

/**
 * A hard-coded map of facet values to aliases.
 *
 * @return array
 *   An array of aliases in the following format:
 *   - [context]
 *   -- [field name] (Pretty paths alias)
 *   --- "solr_field"
 *   ---- [field name] (Original solr field name)
 *   --- "exclusion_method" (optional)
 *   ---- Either "whitelist" or "blacklist"
 *   --- "exclusion_list"
 *   ---- Array of values to either exclude or include.
 *   --- "aliases"
 *   ---- [clean alias] - The desired alias in a machine-readable format
 *   ----- "label"
 *   ------ The desired alias in a human-readable format
 *   ----- "values"
 *   ------ Array of original indexed values to be aliased.
 */
function fcl_fcuk_custom_facets_aliases_list($context = NULL) {
  $fields = [];

  /* Flights */
  $fields['flights']['departure'] = [
    'solr_field' => 'ss_departure',
    'aliases' => [
      'London' => [
        'label' => t('London'),
        'values' => $GLOBALS['conf']['fcluk_london_search_mappings'],
      ],
    ],
  ];
  $fields['flights']['airlines'] = [
    'solr_field' => 'ss_product_supplier',
    'exclusion_method' => 'blacklist',
    'exclusion_list' => [
      'Flight Centre Phone Only Airfare',
    ],
    'aliases' => [
      'KLM' => [
        'label' => t('KLM'),
        'values' => ['KLM ROYAL DUTCH AIRLINES'],
      ],
      'TAP' => [
        'label' => t('TAP'),
        'values' => ['TAP AIR PORTUGAL'],
      ],
      'Easyjet' => [
        'label' => t('Easyjet'),
        'values' => ['EasyJet'],
      ],
      'SriLankan Airlines' => [
        'label' => t('SriLankan Airlines'),
        'values' => ['SRILANKAN AIRLINES'],
      ],
    ],
  ];

  $fields['flights']['type'] = [
    'solr_field' => 'sm_vid_Product_experience',
    'exclusion_method' => 'whitelist',
    'aliases' => [
      'red_label_fares_2_stop' => [
        'label' => t('Red Label Fares - 2 Stop'),
        'values' => ['Red Label Fares- 2 Stop'],
      ],
      'red_label_fares_3_stop' => [
        'label' => t('Red Label Fares - 3 Stop'),
        'values' => ['Red Label Fares- 3 Stop'],
      ],
      'deals_last_minute' => [
        'label' => t('Deals - Last Minute'),
        'values' => ['Deals- Last Minute'],
      ],
      'flight_centre_exclusive' => [
        'label' => t('Flight Centre Exclusive'),
        'values' => ['Deals- Flight Centre Exclusive'],
      ],
      'deals_hot_offer' => [
        'label' => t('Deals - Hot Offer'),
        'values' => ['Deals- Special Offers- hot offer'],
      ],
    ],
  ];

  /* Tours */
  $fields['group-tours']['type'] = [
    'solr_field' => 'sm_vid_Product_experience',
    'exclusion_method' => 'blacklist',
    'exclusion_list' => [
      'Experiences- Historical',
      'Style- Educational',
    ],
    'aliases' => [
      'sightseeing' => [
        'label' => t('Sightseeing'),
        'values' => ['Experiences- Sights'],
      ],
      'active' => [
        'label' => t('Active'),
        'values' => ['Style- Active'],
      ],
      'cultural' => [
        'label' => t('Cultural'),
        'values' => [
          'Experiences- Cultural, Music &amp; Arts',
          'Cultural &amp; Historical- Europe &amp; Russia',
          'Cultural &amp; Historical- Other',
        ],
      ],
      'adventure' => [
        'label' => t('Adventure'),
        'values' => [
          'Style- Adventure',
          'Adventure- Europe &amp; Russia',
          'Adventure- Other',
        ],
      ],
      'outdoors' => [
        'label' => t('Outdoors'),
        'values' => ['Outdoors- Other'],
      ],
      'food_wine' => [
        'label' => t('Food & Wine'),
        'values' => ['Experiences- Culinary- Food &amp; Wine'],
      ],
      'sports' => [
        'label' => t('Sports'),
        'values' => ['Experiences- Outdoors- Sport/Golf/Fishing/Horseriding'],
      ],
      'nature_wildlife' => [
        'label' => t('Nature & Wildlife'),
        'values' => ['Experiences- Nature / Wildlife'],
      ],
      'beaches' => [
        'label' => t('Beaches'),
        'values' => [
          'Experiences- Islands & Beaches',
          'Experiences- Islands &amp; Beaches',
          'Experiences - Islands & Beaches',
          'Experiences - Islands &amp; Beaches',
          'Islands &amp; Beaches - Other',
          'Islands & Beaches - Other',
        ],
      ],
      'special_interest' => [
        'label' => t('Special Interest'),
        'values' => ['Special Intrest'],
      ],
    ],
  ];

  /* Holidays */
  $fields['holidays']['type'] = [
    'solr_field' => 'sm_vid_Product_experience',
    'exclusion_method' => 'whitelist',
    'aliases' => [
      'adventure' => [
        'label' => t('Adventure'),
        'values' => [
          'Adventure- Other',
          'Adventure - Other',
        ],
      ],
      'all_inclusive' => [
        'label' => t('All Inclusive'),
        'values' => ['All Inclusive Packages'],
      ],
      'Beach' => [
        'label' => t('Beach'),
        'values' => [
          'Islands &amp; Beaches- Other',
          'Islands &amp; Beaches - Other',
          'Islands & Beaches- Other',
          'Islands & Beaches - Other',
          'Luxury Holidays- Beach',
          'Luxury Holidays - Beach',
        ],
      ],
      'city_breaks' => [
        'label' => t('City Breaks'),
        'values' => [
          'Short Breaks- Cities',
          'Short Breaks - Cities',
          'Luxury Holidays- City Breaks',
          'Luxury Holidays - City Breaks',
        ],
      ],
      'family' => [
        'label' => t('Family'),
        'values' => ['Family'],
      ],
      'fly_drive' => [
        'label' => t('Fly Drive'),
        'values' => [
          'Fly-drive',
          'Self Drive',
        ],
      ],
      'honeymoon' => [
        'label' => t('Honeymoon'),
        'values' => [
          'Romance &amp; Honeymoons',
          'Luxury Holidays- Honeymoon',
          'Luxury Holidays - Honeymoon',
        ],
      ],
      'last_minute' => [
        'label' => t('Last minute'),
        'values' => [
          'Deals- Last Minute',
          'Deals - Last Minute',
        ],
      ],
      'Luxury' => [
        'label' => t('Luxury'),
        'values' => [
          'Indulgence & Luxury- Other',
          'Indulgence &amp; Luxury- Other',
          'Indulgence & Luxury - Other',
          'Indulgence &amp; Luxury - Other',
        ],
      ],
      'special_offers' => [
        'label' => t('Special Offers'),
        'values' => [
          'Deals- Special Offers',
          'Deals- Special Offers- All inclusive upgrade',
          'Deals- Special Offers - All inclusive upgrade',
          'Deals- Special Offers- Free nights',
          'Deals - Special Offers - Free nights',
          'Deals- Special Offers- Full board upgrade',
          'Deals- Special Offers - Full board upgrade',
          'Deals- Special Offers- Half board upgrade',
          'Deals - Special Offers - Half board upgrade',
          'Deals- Special Offers- Reduced room rate',
          'Deals - Special Offers - Reduced room rate',
          'Deals- Special Offers- Resort credit',
          'Deals - Special Offers - Resort credit',
          'Deals- Special Offers- Room Upgrade',
          'Deals - Special Offers - Room Upgrade',
        ],
      ],
      'summer_sun' => [
        'label' => t('Summer Sun'),
        'values' => ['Summer Holidays'],
      ],
      'winter_sun' => [
        'label' => t('Winter Sun'),
        'values' => [
          'Holiday- Winter Sun',
          'Holiday - Winter Sun',
        ],
      ],
    ],
  ];

  $fields['holidays']['holiday_range'] = [
    'solr_field' => 'ss_product_selection',
    'exclusion_method' => 'whitelist',
    'aliases' => [
      'holiday_package' => [
        'label' => t('Holiday Package'),
        'values' => ['Holiday Package'],
      ],
      'luxury_holidays' => [
        'label' => t('Luxury Holidays'),
        'values' => ['Luxury Holiday Package'],
      ],
      'journeys' => [
        'label' => t('Journeys'),
        'values' => ['RTW Journeys'],
      ],
      'escorted_jouneys' => [
        'label' => t('Escorted Journeys'),
        'values' => ['Escorted Journeys'],
      ],
      'escapes' => [
        'label' => t('Escapes'),
        'values' => ['Escapes'],
      ],
    ],
  ];

  /* Luxury Holidays */
  $fields['luxury_holidays']['type'] = [
    'solr_field' => 'sm_vid_Product_experience',
    'exclusion_method' => 'whitelist',
    'aliases' => [
      'luxury_beach_escapes' => [
        'label' => t('Luxury Beach Escapes'),
        'values' => ['Luxury Holidays- Beach'],
      ],
      'luxury_city_escapes' => [
        'label' => t('Luxury City Escapes'),
        'values' => ['Luxury Holidays- City Breaks'],
      ],
      'luxury_honeymoons' => [
        'label' => t('Luxury Honeymoons'),
        'values' => ['Luxury Holidays- Honeymoon'],
      ],
    ],
  ];

  // Guess context if not supplied based on path.
  if (empty($context)) {
    $url = drupal_get_path_alias();
    switch ($url) {
      case 'group-tours/search':
        $context = 'group-tours';
        break;

      case 'holidays/search':
        $context = 'holidays';
        break;

      case 'first-and-business/luxury-holidays/search':
        $context = 'luxury_holidays';
        break;

      case 'flights/search':
      case 'first-and-business/search':
      default:
        $context = 'flights';
        break;
    }
  }

  if ($context == 'all') {
    return $fields;
  }
  else {
    return $fields[$context];
  }
}

/**
 * Creates a reverse-lookup list of aliases.
 *
 * This function takes the aliases list defined by
 * fcl_fcuk_custom_facets_aliases_list() and produces a backwards version which
 * can be used to more-quickly look up an alias based on a Solr field value.
 *
 * Data is returned in the format:
 *
 * [
 *   // Aliases for the "Type" field.
 *   'type' => [
 *     // "Islands & Luxury - Other" is the value of a Solr field.
 *     'Islands & Luxury - Other'] = [
 *       // Label is the text shown to the user in an <option> tag.
 *       'label' => 'Luxury',
 *       // Value is the value of an <option> tag.
 *       'value' => 'luxury',
 *     ],
 *   ],
 * ]
 *
 * @param string $context
 *   The context, for example "type" for holiday types, "destination" for
 *   holiday destinations, or "star_rating" for you-know-what.
 *
 * @return array
 *   An array containing a reverse mapping of aliases to Solr field values
 *   per the example structure above.
 *
 * @see fcl_fcuk_custom_facets_aliases_list().
 */
function fcl_fcuk_custom_facets_aliases_list_reverse_lookup($context) {
  // Get the list of aliases.
  $aliases_list = fcl_fcuk_custom_facets_aliases_list($context);

  // Initialise an array which will contain our reverse-lookup aliases.
  $aliases_list_reverse_lookup = [];

  // Loop through each alias's Solr values and add them in to the lookup
  // array. For example, if we have a Solr value of "Islands & Luxury - Other"
  // which maps to the alias "Luxury", then we will create an entry in
  // $aliases_list_reverse_lookup like this:
  // $aliases_list_reverse_lookup['type']['Islands & Luxury - Other'] = [
  //   'label' => 'Luxury',
  //   'value' => 'luxury',
  // ];
  foreach ($aliases_list as $url_query_field => $aliases_for_url_query_field) {
    foreach ($aliases_for_url_query_field['aliases'] as $alias => $alias_information) {
      // Loop through each Solr field value and map back to the alias.
      // Confused yet? Me too...
      foreach ($alias_information['values'] as $solr_field_value) {
        $aliases_list_reverse_lookup[$url_query_field][$solr_field_value] = [
          'label' => $alias_information['label'],
          'value' => $alias,
        ];
      }
    }
  }

  // Return the reverse-lookup aliases.
  return $aliases_list_reverse_lookup;
}

/**
 * Get the names of fields that have aliases.
 *
 * @param string $context
 *   (optional) "flights", "holidays", "tours", or "all".
 *
 * @return array
 *   Keys are solr facet names and values are field aliases (pretty paths).
 */
function fcl_fcuk_custom_facets_aliases_get_field_names($context = NULL) {
  $field_names = &drupal_static(__FUNCTION__ . $context);
  if (!isset($field_names)) {
    $field_names = [];
    $aliases = fcl_fcuk_custom_facets_aliases_list($context);

    if ($context == 'all') {
      // All field names, regardless of context. Used for caching.
      $array = [];
      foreach ($aliases as $key => $value) {
        $array = array_merge($array, $value);
      }
      $aliases = $array;
    }

    foreach ($aliases as $k => $v) {
      $field_names[$v['solr_field']] = $k;
    }
  }
  return $field_names;
}

/**
 * Given an indexed value, return the clean alias.
 *
 * @param string $field_name
 *   The field name (pretty path alias, not solr field name).
 * @param string $indexed_value
 *   The original indexed solr value.
 * @param string $context
 *   (optional) "flights", "holidays", "tours", or "all".
 *
 * @return string
 *   The clean alias key (url param key) if it exists or else FALSE.
 */
function fcl_fcuk_custom_facets_aliases_get_clean($field_name, $indexed_value, $context = NULL) {
  $clean_alias = &drupal_static(__FUNCTION__ . $field_name . $indexed_value . $context);
  if (!isset($clean_alias)) {
    $aliases = fcl_fcuk_custom_facets_aliases_list($context);
    $clean_alias = FALSE;
    foreach ($aliases[$field_name]['aliases'] as $clean => $alias) {
      if (in_array($indexed_value, $alias['values'])) {
        $clean_alias = $clean;
      }
    }
  }
  return $clean_alias;
}
