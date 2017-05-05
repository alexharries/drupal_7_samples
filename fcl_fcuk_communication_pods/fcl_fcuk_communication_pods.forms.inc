<?php

/**
 * @file
 * Forms for the Communication Pods module.
 */

/**
 * The callback handler for the edit pod form.
 *
 * @param CommunicationPod|null $pod
 *   The Communication Pod which we want the URI for.
 *
 * @return array
 *   A Drupal form API array.
 */
function fcl_fcuk_communication_pods_edit_pod_callback($pod = NULL) {
  // If we don't have a communication pod, create an empty one now.
  if (empty($pod)) {
    $pod = new stdClass();
    _fcl_fcuk_communication_pods_decorate_empty_pod($pod);
  }

  return drupal_get_form('fcl_fcuk_communication_pods_edit_pod_form', $pod);
}

/**
 * A form to create/edit a Communication Pod.
 *
 * Note that only users with the permission "edit communication pod javascript"
 * can add javascript code or links to external JS files.
 *
 * @param array $form
 *   The FAPI form.
 *
 * @param array $form_state
 *   The form state array.
 *
 * @param CommunicationPod $pod
 *   Either a fully-loaded Communication Pod entity, or a skeleton entity.
 *   If it's a real Communication Pod, the machine name key won't be empty.
 *
 * @return array
 *   A form API array of awesomeness.
 */
function fcl_fcuk_communication_pods_edit_pod_form($form, &$form_state, stdClass $pod) {
  // Set a boolean if we're creating a new pod.
  $creating_new_pod = empty($pod->{COMMUNICATION_POD_MACHINE_NAME});

  // Are we editing an existing pod or creating a new one?
  $form['create_or_edit'] = [
    '#type' => 'value',
    '#value' => $creating_new_pod ? fcl_fcuk_communication_pods_get_action(COMMUNICATION_POD_ACTION_CREATE) : fcl_fcuk_communication_pods_get_action(COMMUNICATION_POD_ACTION_UPDATE, $pod),
  ];

  // If we're editing an existing pod, the machine name needs to be a server-
  // side value; if it's a new pod, this needs to be a textfield.
  if ($creating_new_pod) {
    $form[COMMUNICATION_POD_MACHINE_NAME] = [
      '#type' => 'textfield',
      '#required' => TRUE,
      '#title' => t('Pod Machine Name'),
      '#description' => t('Enter a machine name for the pod made up of lower-case letters, numbers and underscores. It must begin with a letter or number.'),
      '#element_validate' => ['fcl_fcuk_communication_pods_pod_machine_name_validate'],
    ];
  }
  else {
    $form[COMMUNICATION_POD_MACHINE_NAME] = [
      '#type' => 'value',
      '#value' => $pod->{COMMUNICATION_POD_MACHINE_NAME},
    ];
  }

  // Store the pod in a server-side field. If we're creating a new pod, this
  // will be null.
  $form[COMMUNICATION_PODS_ENTITY_NAME] = [
    '#type' => 'value',
    '#value' => $pod,
  ];

  // The pod's title.
  $form[COMMUNICATION_POD_FRIENDLY_NAME] = [
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Pod Friendly Name'),
    '#description' => t('The human-readable name for the pod.'),
    '#default_value' => $pod->{COMMUNICATION_POD_FRIENDLY_NAME},
  ];

  // The pod's HTML.
  $form['pod_html'] = [
    '#type' => 'textarea',
    '#title' => t('Pod HTML'),
    '#description' => t('Enter the HTML for your pod here.'),
    '#default_value' => $pod->pod_html,
  ];

  // The pod's in-line CSS.
  $form['pod_css'] = [
    '#type' => 'textarea',
    '#title' => t('Pod CSS'),
    '#description' => t('You can add custom in-line CSS to your pod here. Your pod will be wrapped in a class in the format <em>.communication-pod--[your pod machine name with underscores converted to hyphens]</em>, for example <em>.communication-pod--my-funky-pod</em>, and you should prefix all your styles with this class to prevent your pod from affecting the styles elsewhere on the page.'),
    '#default_value' => $pod->pod_css,
  ];

  // The pod's external CSS file URLs, one per line.
  $form['pod_external_css'] = [
    '#type' => 'textarea',
    '#title' => t('External CSS files'),
    '#description' => t('If you want to load one or more external CSS files with your pod, add their URLs here, one per line. It is <strong>your</strong> responsibility to ensure any CSS you provide with your pod doesn\'t affect the styling of the rest of the page.'),
    '#default_value' => implode("\r\n", $pod->pod_external_css),
    '#element_validate' => ['fcl_fcuk_communication_pods_pod_external_urls_validate'],
  ];

  // The pod's JS - disabled if the current user doesn't have access to edit
  // pod JS.
  $form['pod_js'] = [
    '#type' => 'textarea',
    '#title' => t('Pod Javascript'),
    '#description' => t('You can add custom in-line javascript to your pod here. This field will be disabled if you do not have permission to edit pods\' javascript files.'),
    '#default_value' => $pod->pod_js,
    '#disabled' => !user_access(COMMUNICATION_POD_PERM_EDIT_JS),
  ];

  // The pod's external JS file URLs, one per line. Also disabled if the
  // current user doesn't have access to edit pod JS.
  $form['pod_external_js'] = [
    '#type' => 'textarea',
    '#title' => t('External JS files'),
    '#description' => t('If you want to load one or more external JS files with your pod, add their URLs here, one per line. This field will be disabled if you do not have permission to edit pods\' javascript files.'),
    '#default_value' => implode("\r\n", $pod->pod_external_js),
    '#disabled' => !user_access(COMMUNICATION_POD_PERM_EDIT_JS),
    '#element_validate' => ['fcl_fcuk_communication_pods_pod_external_urls_validate'],
  ];

  // The paths which the pod is displayed on.
  $form['paths'] = [
    '#type' => 'textarea',
    '#title' => t('Paths to display this pod on'),
    '#description' => t('To show this pod only on a specific set of URLs, enter them one per line below. You can use wildcards - for example, <em>monkeys/are/great/*</em> will show the pod on all pages whose URLs begin with <em>monkeys/are/great/</em>.'),
    '#default_value' => implode("\r\n", $pod->paths),
  ];

  // Is the pod disabled?
  $form['disabled'] = [
    '#type' => 'checkbox',
    '#title' => t('Disabled?'),
    '#description' => t('Check this box to <em>disable</em> the pod.'),
    '#default_value' => $pod->disabled,
  ];

  // Show the owner on the form.
  // @todo: Use the author edit field from node_form() to allow editors to
  // change the pod's owner?
  $form['uid'] = [
    '#type' => 'value',
    '#value' => $pod->uid,
  ];

  $pod_account = NULL;
  if ($pod->uid) {
    $pod_account = user_load($pod->uid);
  }

  $form['user_name_visible'] = [
    '#type' => 'textfield',
    '#title' => t('Owner'),
    '#description' => t('This is the name of the user who created this Communication Pod.'),
    '#default_value' => !empty($pod_account) ? $pod_account->name : t('Nobody'),
    '#disabled' => TRUE,
  ];

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  // Add any fields from Field API.
  field_attach_form(COMMUNICATION_PODS_ENTITY_NAME, $pod, $form, $form_state);

  return $form;
}

/**
 * Validation for the form to create/edit a new pod.
 *
 * - If "create_or_edit" is create, check that the machine name doesn't already
 *   exist.
 * - Checks the HTML, CSS and Javascript entered is well-formed.
 * - Runs any links to external JS or CSS files through check_url().
 *
 * @see https://api.drupal.org/api/drupal/includes%21common.inc/group/sanitization/7.x
 */
function fcl_fcuk_communication_pods_edit_pod_form_validate($form, &$form_state) {
  // Validate Field API values.
  $pod = fcl_fcuk_communication_pods_extract_edit_form_values($form_state['values']);
  field_attach_form_validate(COMMUNICATION_PODS_ENTITY_NAME, $pod, $form, $form_state);

  // If "create_or_edit" is create, check that the machine name doesn't already
  // exist.
  if ($form_state['values']['create_or_edit'] == fcl_fcuk_communication_pods_get_action(COMMUNICATION_POD_ACTION_CREATE)) {
    // Try loading a pod with this machine name.
    if (fcl_fcuk_communication_pods_pod_is_in_db($form_state['values'][COMMUNICATION_POD_MACHINE_NAME])) {
      form_set_error(COMMUNICATION_POD_MACHINE_NAME, t('Sorry, this pod machine is already in use. Please choose another.'));
    }
  }
}

/**
 * Checks that an entered pod machine name is valid.
 *
 * @param array $element
 * @param array $form_state
 * @param array $form
 *
 * @see fcl_fcuk_communication_pods_convert_to_pod_machine_name().
 */
function fcl_fcuk_communication_pods_pod_machine_name_validate($element, &$form_state, $form) {
  if ($element['#value'] !== fcl_fcuk_communication_pods_convert_to_pod_machine_name($element['#value'])) {
    form_error($element, t("This doesn't look like a valid machine name; please enter a machine name containing only lower-case letters, numbers, and underscores (_)."));
  }
}

/**
 * Converts a list of external CSS file URLs to an array.
 *
 * @param array $element
 * @param array $form_state
 * @param array $form
 *
 * @see fcl_uk_utilities_text_field_to_array().
 * @see check_url().
 */
function fcl_fcuk_communication_pods_pod_external_urls_validate(&$element, &$form_state, $form) {
  $element['#value'] = fcl_uk_utilities_text_field_to_array($element['#value']);

  // Check that entered URLs are valid, using check_url().
  if (!empty($element['#value'])) {
    foreach ($element['#value'] as &$url) {
      if (!check_url($url)) {
        form_error($element, t("The URL %url doesn't seem to be valid, or contains illegal characters.", ['%url' => $url]));
      }
    }
  }
}

/**
 * Submit handler for the form to create/edit a new pod.
 *
 * - Runs the pod's HTML through filter_xss().
 * - Saves the pod information in the database using entity_save().
 *
 * @see https://api.drupal.org/api/drupal/includes%21common.inc/group/sanitization/7.x
 */
function fcl_fcuk_communication_pods_edit_pod_form_submit($form, &$form_state) {
  // Run the pod's HTML through filter_xss().
  // filter_xss() is mangling legitimate HTML comments, so we're not using
  // it for now. This code is staying in on the off-chance it can be fixed
  // later. We'll use check_markup() instead and see how we get on with that,
  // instead.
//  $form_state['values']['pod_html'] = filter_xss($form_state['values']['pod_html'], fcl_fcuk_communication_pods_pod_html_allowed_tags());
  $form_state['values']['pod_html'] = check_markup(
    $form_state['values']['pod_html'],
    'full_html',
    $form_state['values'][COMMUNICATION_PODS_ENTITY_NAME]->language,
    FALSE);

  // Save the pod information in the database using entity_save().
  // Validate Field API values.
  $pod = fcl_fcuk_communication_pods_extract_edit_form_values($form_state['values']);
  field_attach_submit(COMMUNICATION_PODS_ENTITY_NAME, $pod, $form, $form_state);

  // If the pod saved correctly, show a happy message; otherwise, show a sad
  // one.
  if ($pod = fcl_fcuk_communication_pods_save($pod)) {
    drupal_set_message(t('The Communication Pod @communication_pod_friendly_name was saved.', [
      '@communication_pod_friendly_name' => $pod->{COMMUNICATION_POD_FRIENDLY_NAME},
    ]), 'status');
  }
  else {
    drupal_set_message(t('Sorry, something went wrong and the Communication Pod @communication_pod_friendly_name was not saved. Please berate your webteam about this.', [
      '@communication_pod_friendly_name' => $pod->{COMMUNICATION_POD_FRIENDLY_NAME},
    ]), 'error');
  }

  $form_state['redirect'] = COMMUNICATION_PODS_ADMIN_URL;
}

/**
 * The callback handler for the delete pod form.
 *
 * @param CommunicationPod $pod
 *   The Communication Pod which we want the URI for.
 *
 * @return array
 *   A Drupal form API array.
 */
function fcl_fcuk_communication_pods_delete_pod_callback($pod) {
  return drupal_get_form('fcl_fcuk_communication_pods_delete_pod_form', $pod);
}

/**
 * A form to delete/revert a pod.
 *
 * This just presents a submit button to confirm you want to delete/revert
 * a pod which is stored in the database.
 *
 * @param array $form
 *   The FAPI form.
 *
 * @param array $form_state
 *   The form state array.
 *
 * @param CommunicationPod $pod
 *   The Communication Pod which we want the URI for.
 *
 * @return array
 *   A form API array of awesomeness.
 */
function fcl_fcuk_communication_pods_delete_pod_form($form, &$form_state, stdClass $pod) {
  // Is the pod in the database?
  if (fcl_fcuk_communication_pods_pod_is_in_db($pod)) {
    // Show a confirmation message.
    $form['are_you_sure_title'] = [
      '#type' => 'markup',
      '#markup' => '<h3>' . t('Are you sure you want to @action the Communication Pod %communication_pod_friendly_name?', [
          '@action' => fcl_fcuk_communication_pods_get_action(COMMUNICATION_POD_ACTION_DELETE, $pod),
          '%communication_pod_friendly_name' => $pod->{COMMUNICATION_POD_FRIENDLY_NAME},
        ]) . '</h3>',
    ];

    $form['are_you_sure_text'] = [
      '#type' => 'markup',
      '#markup' => '<p>' . t('You cannot undo this action - eek!') . '</p>',
    ];

    // Store the pod's machine name in the form.
    $form[COMMUNICATION_POD_MACHINE_NAME] = [
      '#type' => 'value',
      '#value' => $pod->{COMMUNICATION_POD_MACHINE_NAME},
    ];

    // Store the pod in a server-side field.
    $form[COMMUNICATION_PODS_ENTITY_NAME] = [
      '#type' => 'value',
      '#value' => $pod,
    ];

    // Add the submit button.
    $form['submit'] = [
      '#type' => 'submit',
      '#value' => t('Delete pod @pod_friendly_name', ['@pod_friendly_name' => $pod->{COMMUNICATION_POD_FRIENDLY_NAME}]),
    ];
  }
  else {
    // The pod isn't in the database, so it can't be deleted.
    $form['not_possible_dude_title'] = [
      '#type' => 'markup',
      '#markup' => '<h3>' . t("Sorry, the Communication Pod %communication_pod_friendly_name can't be deleted", [
          '%communication_pod_friendly_name' => $pod->{COMMUNICATION_POD_FRIENDLY_NAME},
        ]) . '</h3>',
    ];

    $form['not_possible_dude_text'] = [
      '#type' => 'markup',
      '#markup' => '<p>' . t("This pod is provided by a file and hasn't been overridden. If you want to prevent this pod from showing, please edit it and set it to be disabled.") . '</p>',
    ];
  }

  return $form;
}

/**
 * Submit handler for the form to delete/revert a pod.
 *
 * Delete the pod's record from the database.
 */
function fcl_fcuk_communication_pods_delete_pod_form_submit($form, &$form_state) {
  $pod = fcl_fcuk_communication_pods_load($form_state['values'][COMMUNICATION_POD_MACHINE_NAME]);

  // If the pod saved correctly, show a happy message; otherwise, show a sad
  // one.
  if ($deleted_ok = fcl_fcuk_communication_pods_delete($pod)) {
    drupal_set_message(t('The Communication Pod @communication_pod_friendly_name was deleted/reverted.', [
      '@communication_pod_friendly_name' => $pod->{COMMUNICATION_POD_FRIENDLY_NAME},
    ]), 'status');
  }
  else {
    drupal_set_message(t('Sorry, something went wrong and the Communication Pod @communication_pod_friendly_name was not deleted/reverted. Please berate your webteam about this.', [
      '@communication_pod_friendly_name' => $pod->{COMMUNICATION_POD_FRIENDLY_NAME},
    ]), 'error');
  }

  $form_state['redirect'] = COMMUNICATION_PODS_ADMIN_URL;
}

/**
 * Path checker.
 *
 * A page where you can enter a path and see which Communication Pods will
 * display on that page.
 */
// @todo.

/**
 * Submit handler for the path checker.
 */
// @todo.
