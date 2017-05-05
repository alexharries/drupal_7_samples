<?php

/**
 * @file
 * Database schema information for the fcl_fcuk_communication_pods module.
 */

/**
 * Database schema information.
 *
 * @return mixed
 */
function _fcl_fcuk_communication_pods_schema_definitions() {
  $schema[COMMUNICATION_PODS_SCHEMA_NAME] = [
    'description' => 'Stores custom Communication Pods entered by editors.',
    'fields' => [
      COMMUNICATION_POD_ID_FIELD => [
        'description' => 'The entity ID for a Communication Pod.',
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ],
      COMMUNICATION_POD_MACHINE_NAME => [
        'type' => 'varchar',
        'length' => 64,
        'not null' => TRUE,
        'description' => 'Primary Key: Unique pod machine name.',
      ],
      COMMUNICATION_POD_FRIENDLY_NAME => [
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
        'description' => "The pod's name. This is only shown on the administration pages.",
        'translatable' => TRUE,
      ],
      // @todo: implement language handling. One day; not today. Also,
      // @Andrea - monkeys ;o)
      COMMUNICATION_POD_LANGUAGE_FIELD => [
        'type' => 'varchar',
        'length' => 64,
        'not null' => TRUE,
        'description' => 'The language of the Communication Pod.',
      ],
      'disabled' => [
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
        'size' => 'tiny',
        'description' => 'Pod disabled status. (0 = enabled, 1 = disabled)',
      ],
      'weight' => [
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
        'description' => 'Pod weight within its region.',
      ],
      // Region isn't currently being used, but we have provided
      // it for the use-case where editors might want to say which
      // Communication Pod goes in which region of the page.
      'region' => [
        'type' => 'varchar',
        'length' => 64,
        'not null' => TRUE,
        'default' => '',
        'description' => 'Theme region within which the pod is set.',
      ],
      'paths' => [
        'type' => 'text',
        'not null' => TRUE,
        'description' => 'Contents of the "Paths" textarea; contains a list of paths on which to include/exclude the pod.',
      ],
      // Define the pods' HTML, CSS and JS inserts as 4GB longtexts.
      // @see https://www.drupal.org/docs/7/api/schema-api/data-types
      'pod_html' => [
        'type' => 'text',
        'size' => 'big',
        'description' => "The pod's HTML code.",
        // @todo: extend this module to allow translation.
        // @see: https://www.drupal.org/docs/7/creating-custom-modules/making-your-custom-data-translatable
        'translatable' => TRUE,
      ],
      'pod_css' => [
        'type' => 'text',
        'size' => 'big',
        'description' => "The pod's CSS code.",
      ],
      'pod_external_css' => [
        'type' => 'text',
        'size' => 'big',
        'description' => "Any external CSS files which should be loaded with this pod, stored as a serialised array.",
      ],
      'pod_js' => [
        'type' => 'text',
        'size' => 'big',
        'description' => "The pod's JS code.",
      ],
      'pod_external_js' => [
        'type' => 'text',
        'size' => 'big',
        'description' => "Any external JS files which should be loaded with this pod, stored as a serialised array.",
      ],
      // Define an additional info field which we can use to store any other
      // misc data in a serialised blob format.
      'extra_information' => [
        'type' => 'text',
        'size' => 'big',
        'description' => 'Any other miscellaneous data about the pod in a serialised blob format.',
      ],
      // The UID of the pod's creator.
      'uid' => [
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
        'description' => "The UID of the pod's creator.",
      ],
    ],
    'primary key' => [COMMUNICATION_POD_ID_FIELD],
    'unique keys' => [
      // We create a unique key based on the pod's machine name and language.
      // @todo: does this need to be based on the region, too?
      COMMUNICATION_POD_MACHINE_NAME . '__' . COMMUNICATION_POD_LANGUAGE_FIELD => [
        COMMUNICATION_POD_MACHINE_NAME,
        COMMUNICATION_POD_LANGUAGE_FIELD,
      ],
    ],
    'indexes' => [
      COMMUNICATION_POD_MACHINE_NAME => [COMMUNICATION_POD_MACHINE_NAME],
      'disabled' => ['disabled'],
      'weight' => ['weight'],
    ],
  ];

  return $schema;
}
