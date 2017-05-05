<?php
/**
 * @file
 * FCL Pages template paragraph type definition.
 */

/**
 * Define the paragraph type.
 *
 * The "Don't Use" label is only a backup. Visibility set to 0 means that this
 * paragraph type should never be visible as a choice for content editors.
 */
$plugin = [
  'title' => t('A template paragraph type'),
  'description' => t('A template for creating an FCL UK Pages Paragraphs CTools plugin.'),
  'render_callback' => 'fcluk_ribbons_flights_products_listing_template_render',
  'visible' => 0,
  'fields' => [
    // Title field.
    'title' => [
      'field' => [
        'type' => 'text',
      ],
      'instance' => [
        'label' => t('Title'),
        'settings' => [
          'text_processing' => 0,
        ],
        'widget' => [
          'type' => 'text_textfield',
          'weight' => 0,
        ],
      ],
    ],

    // Image field.
    'image' => [
      'field' => [
        'type' => 'image',
        'indexes' => [
          'fid' => [
            0 => 'fid',
          ],
        ],
        'settings' => [
          'default_image' => 0,
          'uri_scheme' => 'public',
        ],
        'entity_types' => [],
      ],
      'instance' => [
        'required' => 0,
        'label' => t('Image (optional)'),
        'widget' => [
          'type' => 'media_generic',
          'weight' => 1,
          'module' => 'media',
          'settings' => [
            'allowed_schemes' => [
              'module' => 0,
              'public' => 'public',
            ],
            'allowed_types' => [
              'audio' => 0,
              'document' => 0,
              'image' => 'image',
              'video' => 0,
            ],
          ],
        ],
      ],
    ],

    // Description field.
    'description' => [
      'field' => [
        'type' => 'text_long',
      ],
      'instance' => [
        'label' => t('Description'),
        'widget' => [
          'module' => 'text',
          'weight' => 2,
          'settings' => [
            'rows' => 5,
          ],
          'type' => 'text_textarea',
        ],
        'settings' => [
          'display_summary' => TRUE,
          'text_processing' => 1,
        ],
        'display' => [
          'default' => [
            'label' => 'hidden',
            'module' => 'text',
            'settings' => [],
            'type' => 'text_default',
          ],
          'paragraphs_editor_preview' => [
            'label' => 'hidden',
            'module' => 'text',
            'settings' => [],
            'type' => 'text_default',
          ],
        ],
      ],
    ],
  ],
];

/**
 * Render callback.
 */
function fcluk_ribbons_flights_products_listing_template_render($entity) {
  $render_array = [];
  
  $render_array['template_widget'] = [
    '#markup' => '<div class="template">' . t('Monkeys') . '</div>'
      . '<pre>' . print_r($entity, TRUE) . '</pre>',
  ];
  
  return $render_array;
}
