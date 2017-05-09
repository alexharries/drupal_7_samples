<?php

/**
 * @file
 * Functions for the Flight Centre UK Communication Pods module.
 */

/**
 * Gets a list of all communication pods in the ./pods directory.
 *
 * This function gets all directories in ./pods which contain a pod.info file.
 *
 * The pod.info file format needs to contain at the minimum a 'pod_name' key,
 * with the pod's human-readable name as its value.
 *
 * The following optional values can be provided:
 *
 * pod_disabled: either 1 (disabled) or 0 (not disabled).
 *
 * paths[]: a path on which to show the pod.
 *
 *   For example, if you want to show the pod on /node/123 and /monkey/banana,
 *   enter one paths[] line for each path, like this:
 *
 *   paths[]: /node/123
 *   paths[]: /monkey/banana
 *
 * pod_external_css: an array of URLs of external CSS files. These will be
 * added as CSS <link /> tags to the pod's HTML.
 *
 * pod_external_js: an array of URLs of external JS files. These will be
 * added as asynchronous JS <script /> tags to the pod's HTML.
 *
 * @param array $pods_to_load
 *   An array of zero or more pods to return. If an empty array is passed,
 *   all known pods will be returned.
 *
 * @param bool $reset
 *   Set to TRUE to override any caching. This is useful when updating the
 *   theme hooks for this module.
 *
 * @return array
 *   An array of zero or more communication pods. The keys of the array will
 *   be the pods' machine names, while the values will be an array of data
 *   from the pod's .info file.
 */
function _fcl_fcuk_communication_pods_load_from_disk(array $pods_to_load = [], $reset = FALSE) {
  // If $pods_to_load is empty, get a list of all pods.
  if (empty($pods_to_load)) {
    $pods_to_load = _fcl_fcuk_communication_pods_list_pods_on_disk();
  }

  // Work out the path to this module and scan for pods.
  $module_path = DRUPAL_ROOT . '/' . drupal_get_path('module', 'fcl_fcuk_communication_pods');
  $pods_directory = $module_path . '/pods';

  // Initialise the data array.
  $pods_loaded_from_disk = &drupal_static(__FUNCTION__, []);

  foreach ($pods_to_load as $pod_machine_name) {
    // Has the pod already been loaded?
    if (!array_key_exists($pod_machine_name, $pods_loaded_from_disk)) {
      // No - load now. Start by setting an empty value so we don't repeatedly
      // attempt to load a non-existent pod.
      $pods_loaded_from_disk[$pod_machine_name] = FALSE;

      $cache_id = 'communication-pods-on-disk:' . $pod_machine_name;

      if (!$reset && variable_get('cache') && ($cache = cache_get($cache_id)) && !empty($cache->data)) {
        $pods_loaded_from_disk[$pod_machine_name] = $cache->data;
      }
      else {
        // Check whether the pod contains a valid .info file.
        $pod_info_file_path = $pods_directory .
          '/' . $pod_machine_name .
          '/' . $pod_machine_name . COMMUNICATION_POD_INFO_EXTENSION;

        // Check that the info file exists.
        if (file_exists($pod_info_file_path)) {
          $pod_info = (array) drupal_parse_info_file($pod_info_file_path);

          // Check that the info file has a pod_name.
          if (array_key_exists(COMMUNICATION_POD_FRIENDLY_NAME, $pod_info)) {
            // Convert the pod to an object.
            $pod_info = (object) $pod_info;

            // Implement a
            // hook_fcl_fcuk_communication_pods_parse_info_file_alter.
            drupal_alter('fcl_fcuk_communication_pods_parse_info_file', $pod_info, $pod_machine_name);

            if (variable_get('cache')) {
              cache_set($cache_id, $pod_info, 'cache', REQUEST_TIME + COMMUNICATION_POD_CACHE_LIFETIME);
            }

            $pods_loaded_from_disk[$pod_machine_name] = $pod_info;
          }
        }
      }
    }
  }

  // Implement hook__fcl_fcuk_communication_pods_load_from_disk_alter
  // to allow other functions/modules to modify this list.
  drupal_alter('_fcl_fcuk_communication_pods_load_from_disk', $pods_loaded_from_disk, $pods_to_load);

  // Now build an array containing only the requested pods.
  $requested_pods = [];

  foreach ($pods_to_load as $pod_to_load) {
    if (!empty($pods_loaded_from_disk[$pod_to_load])) {
      $requested_pods[$pod_to_load] = $pods_loaded_from_disk[$pod_to_load];
    }
  }

  return $requested_pods;
}

/**
 * Gets a list of communication pods in fcl_fcuk_communication_pods/pods/.
 *
 * @return array
 *   An indexed array of zero or more communication pod machine names.
 */
function _fcl_fcuk_communication_pods_list_pods_on_disk() {
  $pods_on_disk = &drupal_static(__FUNCTION__);

  if (is_null($pods_on_disk)) {
    $module_path = DRUPAL_ROOT . '/' . drupal_get_path('module', 'fcl_fcuk_communication_pods');
    $pods = (array) glob($module_path . '/pods/*', GLOB_ONLYDIR);
    $pods_on_disk = [];

    foreach ($pods as $pod_directory) {
      // Get the machine name of the communication pod; this is the directory
      // for the pod.
      $pods_on_disk[] = basename($pod_directory);
    }
  }

  return $pods_on_disk;
}

/**
 * Checks if a communication pod exists on disk or only in the database.
 *
 * @param CommunicationPod $pod
 *
 * @return bool
 *   TRUE if the pod can be found in the pods directory; FALSE if not.
 */
function fcl_fcuk_communication_pods_pod_is_on_disk($pod) {
  $pods_on_disk = _fcl_fcuk_communication_pods_list_pods_on_disk();

  return array_search($pod->{COMMUNICATION_POD_MACHINE_NAME}, $pods_on_disk) !== FALSE;
}

/**
 * Check if a Communication Pod exists in the database.
 *
 * @param CommunicationPod $pod
 *
 * @return bool
 *   TRUE if the pod exists in the database; FALSE if not.
 */
function fcl_fcuk_communication_pods_pod_is_in_db($pod) {
  $pod = fcl_fcuk_communication_pods_load_from_db($pod->{COMMUNICATION_POD_MACHINE_NAME});

  return !empty($pod);
}

/**
 * Gets the status of a Communication Pod.
 *
 * @param string $pod_machine_name
 *   The pod's machine name.
 *
 * @return string|false
 *   The pod's status (see below for status types); FALSE if not found.
 *
 * @see fcl_fcuk_communication_pods.constants.inc
 * @see COMMUNICATION_POD_STATUS_CODE_DEFAULT
 * @see COMMUNICATION_POD_STATUS_CODE_OVERRIDDEN
 * @see COMMUNICATION_POD_STATUS_DB
 */
function fcl_fcuk_communication_pods_pod_status($pod_machine_name) {
  $status = COMMUNICATION_POD_NOT_FOUND;

  // Does the pod exist?
  if ($pod = fcl_fcuk_communication_pods_load($pod_machine_name)) {
    // If the pod is in the database...
    if (fcl_fcuk_communication_pods_pod_is_in_db($pod)) {
      // ... and on disk, it's overridden.
      if (fcl_fcuk_communication_pods_pod_is_on_disk($pod)) {
        $status = COMMUNICATION_POD_STATUS_CODE_OVERRIDDEN;
      }
      // ... but not on disk, it's custom.
      else {
        $status = COMMUNICATION_POD_STATUS_DB;
      }
    }
    else {
      // It's on disk only, so it's file, unaltered.
      $status = COMMUNICATION_POD_STATUS_CODE_DEFAULT;
    }
  }

  // Implement a hook_fcl_fcuk_communication_pods_pod_status_alter.
  drupal_alter('fcl_fcuk_communication_pods_pod_status', $status, $pod);

  return $status;
}

/**
 * Get a list of all enabled communication pods.
 *
 * @param bool $reset
 *   If set to true, will bust the static and db caches and regenerate the
 *   list of pods.
 *
 * @return array
 *   An array of zero or more communication pods.
 */
function fcl_fcuk_communication_pods_list_enabled_pods($reset = FALSE) {
  $enabled_pods = &drupal_static(__FUNCTION__);

  if (!is_array($enabled_pods)) {
    $all_pods = fcl_fcuk_communication_pods_load_multiple([], [], $reset);

    $enabled_pods = [];

    foreach ($all_pods as $pod_machine_name => $pod_info) {
      if (!$pod_info->disabled) {
        $enabled_pods[$pod_machine_name] = $pod_info;
      }
    }
  }

  // Implement a hook_fcl_fcuk_communication_pods_list_enabled_pods_alter.
  drupal_alter('fcl_fcuk_communication_pods_list_enabled_pods', $enabled_pods, $all_pods, $reset);

  return $enabled_pods;
}

/**
 * Returns a list of communication pod machine names which are valid for $path.
 *
 * This function checks which communication pods are available. Once calculated,
 * results are cached for 15 minutes.
 *
 * Note that $path can be either an aliased or an unaliased path; this function
 * will match both aliased and unaliased versions of $path.
 *
 * @param string $path
 *   A Drupal path. If none provided, the value of current_path() is used.
 *
 * @param bool $randomise
 *   If true, the results for the path will be randomly sorted before saving.
 *
 * @return array
 *   An indexed (not associative) array of communication pods which may be
 *   shown at the given path. If no pods are available, an empty array is
 *   returned.
 */
function fcl_fcuk_communication_pods_get_pods_for_path($path = NULL, $randomise = FALSE) {
  // If no path has been provided, use current_path().
  if (is_null($path)) {
    $path = current_path();
  }

  // Get an explicitly aliased and unaliased versions of the path.
  $path_aliased = drupal_get_path_alias($path);
  $path_unaliased = drupal_get_normal_path($path);

  // Initialise our local static array.
  $pods_for_paths = &drupal_static(__FUNCTION__, []);

  // Have we already loaded/calculated which pods are available on this path?
  if (!array_key_exists($path, $pods_for_paths)) {
    // No. Try and load from the cache; build a cache ID.
    $cache_id = 'fcl_fcuk_communication_pods_for_path::' . str_replace('/', ':', $path);

    if (variable_get('cache') && ($pods_for_path_cache_result = cache_get($cache_id))
      && $pods_for_path_cache_result->data != NULL
    ) {
      $pods_for_paths[$path] = $pods_for_path_cache_result->data;
    }
    else {
      // Not yet built; build now.
      $pods_for_paths[$path] = [];

      // Loop through all available pods.
      foreach (fcl_fcuk_communication_pods_load_multiple() as $pod_machine_name => $pod_info) {
        // If the pod is not enabled, skip it.
        if ($pod_info->disabled) {
          continue;
        }

        // Implode any paths defined for the pod into a single string so we can
        // use drupal_match_path().
        $paths_imploded = implode("\r\n", $pod_info->paths);

        // Check if the pod is valid for the path or its alias/"unalias".
        if (drupal_match_path($path_aliased, $paths_imploded) || drupal_match_path($path_unaliased, $paths_imploded)) {
          $pods_for_paths[$path][] = $pod_info;
        }
      }

      // If the pods are to be randomly-sorted, do that now.
      if ($randomise) {
        shuffle($pods_for_paths[$path]);
      }

      // Implement hook_fcl_fcuk_communication_pods_list_pods_for_path_alter
      // to allow other functions/modules to modify this list.
      drupal_alter('fcl_fcuk_communication_pods_list_pods_for_path', $pods_for_paths[$path], $path, $randomise);

      // Save the list if caching is enabled.
      if (variable_get('cache')) {
        cache_set($cache_id, $pods_for_paths[$path], 'cache', REQUEST_TIME + COMMUNICATION_POD_CACHE_LIFETIME);
      }
    }
  }

  // Send the list back, if we have anything.
  return $pods_for_paths[$path];
}

/**
 * Gets the machine name of the nth communication pod for $path.
 *
 * @param null|string $path
 *   The Drupal path to get pods for. If NULL, the value of current_path() will
 *   be used.
 *
 * @param int $pod_number
 *   The position of the pod to get from the list of pods, starting at 0.
 *   If no pod is available, this function returns FALSE.
 *
 * @return string
 *   The machine name of the pod, or FALSE if none available.
 */
function fcl_fcuk_communication_pods_get_pod_for_path($path = NULL, $pod_number = 0) {
  // Check if the pod exists.
  if ($pods_for_path = fcl_fcuk_communication_pods_get_pods_for_path($path)) {
    if (array_key_exists($pod_number, $pods_for_path)) {
      return $pods_for_path[$pod_number];
    }
  }

  return FALSE;
}

/**
 * Checks whether a pod is enabled and can be shown.
 *
 * @param string $pod_machine_name
 *   The pod's machine name; i.e. the directory name of the pod.
 *
 * @return bool
 *   TRUE if the pod is enabled, as determined by having a valid pod.info file,
 *   or FALSE if not.
 */
function fcl_fcuk_communication_pods_pod_enabled($pod_machine_name) {
  // Start by assuming the pod is disabled.
  $pod_enabled = FALSE;

  // Load the pod.
  if ($pod = fcl_fcuk_communication_pods_load($pod_machine_name)) {
    // Pods only keep track of whether they are _disabled_, not enabled, so we
    // invert the value of its disabled key.
    $pod_enabled = !$pod->disabled;

    // Implement hook_fcl_fcuk_communication_pods_pod_enabled_alter
    // to allow other functions/modules to modify this result.
    drupal_alter('fcl_fcuk_communication_pods_pod_enabled', $pod_enabled, $pod_machine_name, $valid_communication_pods);
  }

  return $pod_enabled;
}

/**
 * Gets information about a communication pod's web path, CSS and JS.
 *
 * @todo: This docblock needs a re-write, and it might be worth refactoring
 * this function into the caller function or at least having a tidy.
 *
 * For a valid communication pod on disk, this function decorates $pod with
 * the pod's information.
 *
 * This function also checks for the presence of pod.js or pod.css files
 * and includes them in the pod's HTML output.
 *
 * @param \stdClass $pod
 *   An object containing at a minimum a pod_machine_name value with the pod's
 *   machine name.
 *
 *   This function then decorates $pod with further information about the pod,
 *   including its HTML, CSS and JS data, the pod's absolute server-side path,
 *   its path relative to the Drupal root, and its URL relative to the
 *   webserver's root for the construction of URLs, for example to link to
 *   images in the pod's directory. It also contains any in-line CSS and
 *   JS found.
 *
 *    [
 *      'pod_absolute_path' =>
 *      '/var/webroot/drupal-subdirectory/sites/all/modules/custom/communication_pods/pods/monkeys',
 *      'pod_relative_path' =>
 *      'sites/all/modules/custom/communication_pods/pods/monkeys',
 *      'pod_url' =>
 *      '/drupal-subdirectory/sites/all/modules/custom/communication_pods/pods/monkeys',
 *      'pod_css' => '<style type="text/css"> @import
 *      url('/drupal-subdirectory/sites/all/modules/custom/communication_pods/pods/monkeys/monkeys.css');
 *      </style>',
 *      'pod_js' => '<script
 *      src="/drupal-subdirectory/sites/all/modules/custom/communication_pods/pods/monkeys/monkeys.js"></script>',
 *    ];
 *
 *   For example, if the site is hosted at www.example.com, and this
 *   communication pod is in:
 *
 *   [Drupal webroot]/sites/all/modules/custom/communication_pods/pods/monkeys
 *
 *   .. then the value of pod_url will be:
 *
 *   /sites/all/modules/custom/communication_pods/pods/monkeys
 *
 *   If Drupal is installed in a subdirectory, at
 *   [server webroot]/my-awesome-drupal-website, which means the Drupal
 *   homepage would be at:
 *
 *   www.example.com/my-awesome-drupal-website
 *
 *   ... and this communication pod is in:
 *
 *   [Drupal webroot]/sites/all/modules/custom/communication_pods/pods/monkeys
 *
 *   ... then the value of pod_url will be:
 *
 *   /my-awesome-drupal-website/sites/all/modules/custom/communication_pods/pods/monkeys
 *
 *   If you have an image at ./images/monkey.jpg, you could then use this path
 *   to create an image link like this:
 *
 *   <img src="<?php print $pod_path ?>/images/monkey.jpg" alt="<?php print
 *   t('A monkey') ?>" title="<?php print t('A picture of a monkey hanging onto
 *   a drainpipe.') ?>" />
 *
 * @param string $pod_machine_name
 *   The pod's machine name.
 */
function fcl_fcuk_communication_pods_initialise_pod(stdClass &$pod, $pod_machine_name) {
  $initialised_pods = &drupal_static(__FUNCTION__, []);

  // Has this pod already been initialised?
  if (!array_key_exists($pod_machine_name, $initialised_pods)) {
    // Add the pod's machine name.
    $pod->{COMMUNICATION_POD_MACHINE_NAME} = $pod_machine_name;

    // Get the absolute path to the pod, but only if it's on disk.
    $pod->pod_absolute_path = FALSE;
    $pod_absolute_path_not_checked = DRUPAL_ROOT . '/' . drupal_get_path('module', 'fcl_fcuk_communication_pods') . '/pods/' . $pod_machine_name;

    // If the pod exists on disk, add some extra information about it.
    if (is_dir($pod_absolute_path_not_checked)) {
      // Put the pod's absolute path in.
      $pod->pod_absolute_path = $pod_absolute_path_not_checked;

      // If the pod is in the database, we don't want to over-write any
      // of its data with information from disk so we skip loading in its
      // CSS and JS, etc.
      $pod_is_in_db = fcl_fcuk_communication_pods_pod_is_in_db($pod);

      // Determine the path of the pod relative to Drupal's root.
      $pod->pod_relative_path = drupal_get_path('module', 'fcl_fcuk_communication_pods') . '/pods/' . $pod_machine_name;

      // Determine the pod's URL.
      $pod->pod_url = url($pod->pod_relative_path);

      // Check if there's a pod.css or pod.js file in the communication pod and,
      // if present, add in-line. Adding in-line allows us to include
      // javascript and CSS when we're not loading a pod as part of a complete
      // Drupal page load, for example when we're serving a pod via Ajax/Ahah.

      // If the CSS file exists, get its contents.
      $pod->pod_css_absolute_path = $pod->pod_absolute_path . '/' . $pod->{COMMUNICATION_POD_MACHINE_NAME} . COMMUNICATION_POD_CSS_EXTENSION;
      if (!$pod_is_in_db && file_exists($pod->pod_css_absolute_path)) {
        $pod->pod_css = file_get_contents($pod->pod_css_absolute_path);
      }

      // If the JS file exists, get its contents.
      $pod->pod_js_absolute_path = $pod->pod_absolute_path . '/' . $pod->{COMMUNICATION_POD_MACHINE_NAME} . COMMUNICATION_POD_JS_EXTENSION;
      if (!$pod_is_in_db && file_exists($pod->pod_js_absolute_path)) {
        $pod->pod_js = file_get_contents($pod->pod_js_absolute_path);
      }

      // Load the pod's .tpl.html file, allow the html to be altered, and then
      // run variaboo replacements on it in the preprocess function which
      // called this function.
      $pod->pod_template_absolute_path = $pod->pod_absolute_path . '/' . $pod->{COMMUNICATION_POD_MACHINE_NAME} . COMMUNICATION_POD_TEMPLATE_EXTENSION;
      if (!$pod_is_in_db && file_exists($pod->pod_template_absolute_path)) {
        $pod->pod_html = file_get_contents($pod->pod_template_absolute_path);
      }
    }

    // Add a theme function.
    $pod->theme = COMMUNICATION_POD_THEME_FUNCTION;

    // Always set the 'disabled' key on the pod, even if it's not disabled.
    if (empty($pod->disabled)) {
      $pod->disabled = FALSE;
    }

    // Convert numeric 0 or 1 to FALSE or TRUE as provided by the info file.
    if ($pod->disabled === 0) {
      $pod->disabled = FALSE;
    }

    if ($pod->disabled === 1) {
      $pod->disabled = TRUE;
    }

    // Disable pods whose machine name ends in '__disabled'.
    $pod_machine_name_substr = substr($pod_machine_name, (0 - strlen(COMMUNICATION_POD_POD_DISABLED)));
    if ($pod_machine_name_substr == COMMUNICATION_POD_POD_DISABLED) {
      $pod->disabled = TRUE;
    }

    // If the pod doesn't have a paths array, create one now with a single *
    // entry.
    if (empty($pod->paths)) {
      $pod->paths = ['*'];
    }

    // Make sure all the values we're expecting on the pod are set.
    _fcl_fcuk_communication_pods_decorate_empty_pod($pod);

    // Implement a hook_fcl_fcuk_communication_pods_initialise_pod_alter.
    drupal_alter('fcl_fcuk_communication_pods_initialise_pod', $pod);

    $initialised_pods[$pod_machine_name] = $pod;
  }
}

/**
 * Helper function to issue the theme call to build a communication pod.
 *
 * @param string $pod_machine_name
 *   The communication pod's machine name.
 *
 * @return string
 *   The HTML output of a pod.
 */
function fcl_fcuk_communication_pods_render_pod($pod_machine_name) {
  // @todo: preprocess the pod here.

  // When we render out the CSS textarea contents, we need to wrap it in:
  // "<style type=\"text/css\"> " .  . " </style>"

//      // If the pod specifies external CSS files, add them to the pod's CSS
//      // now.
//      if (!empty($pod_info_file->pod_external_css)) {
//        // Make sure it's an array.
//        $pod_info_file->pod_external_css = (array) $pod_info_file->pod_external_css;
//
//        foreach ($pod_info_file->pod_external_css as $pod_external_css_file) {
//          $initialised_pods[$pod_machine_name]->pod_css .= '<link type="text/css" rel="stylesheet" href="' . $pod_external_css_file . '" media="screen" />' . "\r\n";
//        }
//      }

  // When we render out the JS textarea contents, we need to wrap it in:
  // '<script type="text/javascript"> ' .  . ' </script>'

//      // If the pod specifies external JS files, add them to the pod's JS
//      // now.
//      if (!empty($pod_info_file->pod_external_js)) {
//        // Make sure it's an array.
//        $pod_info_file->pod_external_js = (array) $pod_info_file->pod_external_js;
//
//        foreach ($pod_info_file->pod_external_js as $pod_external_js_file) {
//          $initialised_pods[$pod_machine_name]->pod_js .= '<script type="text/javascript" src="' . $pod_external_js_file . '" async></script>' . "\r\n";
//        }
//      }

  $monkeys = 'TRUE';

  return render(fcl_fcuk_communication_pods_load($pod_machine_name));
}

/**
 * Preprocess a communication pod.
 *
 * @param array $variables
 *   The theming variables for the pod.
 */
function fcl_fcuk_communication_pods_preprocess_pod(&$variables) {
  // Merge in any defaults.
  $variables += [
    'attributes_array' => [],
    'title_attributes_array' => [],
    'content_attributes_array' => [],
    'classes_array' => [],
    'css' => '',
    'js' => '',
  ];

  // Get the path to the pod's files, and any CSS and JS included in the pod.
  // This call adds the following variables for templates:
  //
  // - pod_url: URL of the pod's files relative to the webserver root to allow
  //   pods to inject images,
  // - pod_css: a string of HTML containing the pod's CSS file, if one is
  //   present,
  // - pod_js: a string of HTML containing the pod's JS file, if present.
  // - pod_absolute_path: the absolute path to the pod's files, relative to
  //   the filesystem root (so you can include files from the pod's directory,
  //   if you wanted to).
  // - pod_relative_path: the pod's path relative to Drupal's index.php; this
  //   can be run through url() to give you a path relative to the top of the
  //   domain.
  // - pod_html: the contents of the pod's .tpl.html file, with any {variables}
  //   unchanged.
  if ($pod = fcl_fcuk_communication_pods_load($variables[COMMUNICATION_POD_MACHINE_NAME])) {
    // Add classes to the element which we can use to wrap the pod.
    $variables['classes_array'][] = 'communication-pod';
    $variables['classes_array'][] = 'communication-pod--' . fcl_fcuk_communication_pods_machine_name_to_class($variables[COMMUNICATION_POD_MACHINE_NAME]);

    // Run variable replacements on the pod's HTML, CSS and JS variaboos.
    foreach ($pod as $pod_variable_name => $pod_variable_value) {
      // Don't attempt to process {pod_html}, {pod_css} or {pod_js}, due to an
      // irrational fear of finding ourselves in a funky loop of dooooom...
      if (in_array($pod_variable_name, [
        'pod_html',
        'pod_css',
        'pod_js',
      ])) {
        continue;
      }

      // We can't replace a string with an array/object, so if
      // $pod_variable_value is array|object, implode it into a comma-separated
      // string.
      if (is_object($pod_variable_value) || is_array($pod_variable_value)) {
        $pod_variable_value = implode(', ', $pod_variable_value);
      }

      $pod->pod_html = str_replace('{' . $pod_variable_name . '}', $pod_variable_value, $pod->pod_html);
      $pod->pod_css = str_replace('{' . $pod_variable_name . '}', $pod_variable_value, $pod->pod_css);
      $pod->pod_js = str_replace('{' . $pod_variable_name . '}', $pod_variable_value, $pod->pod_js);
    }

    // Now we've processed all the variaboos, loop through each and copy them
    // into the $variables array.
    foreach ($pod as $pod_variable_name => $pod_variable_value) {
      // Merge the pod's variables into the variables array.
      $variables[$pod_variable_name] = $pod_variable_value;
    }

    // Add the pod itself in as a variable in $variables.
    $variables[COMMUNICATION_POD_ENTITY_NAME] = $pod;
  }

  // Implement a hook_fcl_fcuk_communication_pods_preprocess_pod_alter.
  // Note that this will run even when the pod isn't found.
  drupal_alter('fcl_fcuk_communication_pods_preprocess_pod', $variables);
}

/**
 * Converts a pod's machine name to a CSS-friendly format.
 *
 * This function replaces underscores in the pod's machine name with hyphens,
 * and converts the name to lower case.
 *
 * @param string $pod_machine_name
 *   The pod's machine name. This should be lowercase already, and only
 *   contain letters, numbers and underscores.
 *
 * @return string
 *   The pod's machine name, lower case, with underscores converted to hyphens.
 */
function fcl_fcuk_communication_pods_machine_name_to_class($pod_machine_name) {
  return drupal_strtolower(str_replace('_', '-', $pod_machine_name));
}

/**
 * Title callback for edit/delete communication pod pages.
 *
 * @param string $action
 *   The action to be performed on the pod, for example "edit" or "delete".
 *
 * @param CommunicationPod $pod
 *
 * @return null|string
 *   The title for the page.
 */
function fcl_fcuk_communication_pods_title_callback($action, $pod) {
  // If $action is delete and the pod exists on disk, change the word to
  // "revert".
  $action = fcl_fcuk_communication_pods_get_action($action, $pod);

  return t('@action Communication Pod @pod_friendly_name', [
    '@action' => drupal_ucfirst(t($action)),
    '@pod_friendly_name' => $pod->{COMMUNICATION_POD_FRIENDLY_NAME},
  ]);
}

/**
 * Alter the $action text for certain types of Communication Pod.
 *
 * @param string $action
 *   The action, e.g. create, edit or delete.
 *
 * @param CommunicationPod|null $pod
 *
 * @return string
 *   The action, which may have been changed, or it may be unchanged. Who knows?
 */
function fcl_fcuk_communication_pods_get_action($action, $pod = NULL) {
  // If action is read, we change it to "view".
  if ($action == COMMUNICATION_POD_ACTION_READ) {
    $action = 'view';
  }

  // If $action is edit and the pod doesn't exist in the database, change it
  // to override.
  if (!empty($pod) && ($action == COMMUNICATION_POD_ACTION_UPDATE) && !fcl_fcuk_communication_pods_pod_is_in_db($pod)) {
    $action = COMMUNICATION_POD_ACTION_OVERRIDE;
  }

  // If $action is delete and the pod exists on disk, change the word to
  // "revert".
  if (!empty($pod) && ($action == COMMUNICATION_POD_ACTION_DELETE) && fcl_fcuk_communication_pods_pod_is_on_disk($pod)) {
    $action = COMMUNICATION_POD_ACTION_REVERT;
  }

  return $action;
}

/**
 * Displays a list of all Communication Pods their configuration settings.
 */
function fcl_fcuk_communication_pods_test_page() {
  // If the URL contains 'nocache', bypass the cache, then redirect back to
  // this page.
  if (isset($_GET['nocache'])) {
    fcl_fcuk_communication_pods_load_multiple([], [], TRUE);
    drupal_set_message(t('Communication Pods cache reset'), 'status');
    drupal_goto(current_path());
  }

  // Initialise the render array.
  $render = [];

  // Create a title.
  $render['title'] = [
    '#type' => 'html_tag',
    '#tag' => 'h1',
    '#value' => t('Communication Pods Preview'),
  ];

  $render['description_line_1'] = [
    '#type' => 'html_tag',
    '#tag' => 'p',
    '#value' => t('This page shows an example of all available Communication Pods for this site, including their configuration information.'),
  ];

  $render['description_line_2'] = [
    '#type' => 'html_tag',
    '#tag' => 'p',
    '#value' => t('Note that these pods are cached; <a href="@flush_cache_link">you can flush this cache and rebuild the pods - for example, if you install a new pod - here</a>.', [
      '@flush_cache_link' => url(current_path(), ['query' => ['nocache' => TRUE]]),
    ]),
  ];

  $pods = fcl_fcuk_communication_pods_load_multiple();

  foreach (element_children($pods) as $pod_machine_name) {
    // Create a horizontal line to separate the pod.
    $render[$pod_machine_name . '_separator'] = [
      '#type' => 'html_tag',
      '#tag' => 'hr',
    ];

    // Create a heading for the pod.
    $render[$pod_machine_name . '_title'] = [
      '#type' => 'html_tag',
      '#tag' => 'h3',
      '#value' => t('Communication pod %pod_name', ['%pod_name' => $pods[$pod_machine_name]->pod_name]),
    ];
    // Now output the pod itself. Note that we have to use clone() here to
    // avoid PHP's default behaviour to store a reference to a pod (which would
    // mean later pods would overwrite earlier pods).
    $render[$pod_machine_name] = fcl_fcuk_communication_pods_get_render_array($pods[$pod_machine_name]);

    // Lastly, put the pod's .info file contents in a textarea.
    $render[$pod_machine_name . '_textarea_title'] = [
      '#type' => 'html_tag',
      '#tag' => 'h4',
      '#value' => t('Info file %pod_machine_name%info_file_extension contents:', [
        '%pod_machine_name' => $pod_machine_name,
        '%info_file_extension' => COMMUNICATION_POD_INFO_EXTENSION,
      ]),
    ];

    $render[$pod_machine_name . '_info'] = [
      '#type' => 'textarea',
      '#value' => print_r($pods[$pod_machine_name], TRUE),
    ];
  }

  return $render;
}

/**
 * Builds the admin overview page for all Communication Pods.
 *
 * @todo: Move into a separate .pages.inc file?
 * @todo: Add a pager.
 */
function fcl_fcuk_communication_pods_admin_page() {
  // If the user has access to create pods, create a link to do that now.
  $add_pod_link = '';

  if (fcl_fcuk_communication_pods_access(COMMUNICATION_POD_ACTION_CREATE)) {
    $add_pod_link = '<p>' . t('<a href="@create_url">+ Create Communication Pod</a>', [
        '@create_url' => url(COMMUNICATION_POD_ADMIN_URL . '/' . COMMUNICATION_POD_ACTION_CREATE),
      ]) . '</p>';
  }

  // Initialise the render array.
  $render = [];

  // Create a title.
  $render['title'] = [
    '#theme' => 'html_tag',
    '#tag' => 'h1',
    '#value' => t('Communication Pods'),
  ];

  $render['description_line_1'] = [
    '#theme' => 'html_tag',
    '#tag' => 'p',
    '#value' => t('This page shows a list of all available Communication Pods.') . $add_pod_link,
  ];

  $render['help_text_pods_overview'] = [
    '#markup' => fcl_fcuk_communication_pods_help('help_text_pods_overview'),
  ];

  // Get a list of all pods.
  $all_pods = fcl_fcuk_communication_pods_load_multiple();

  // If we have pods, sort them by machine name and build a table render array.
  if (!empty($all_pods)) {
    $render['pods_table'] = [
      '#theme' => 'table',
      '#caption' => t(''),
      '#header' => [
        // The pod's name column.
        t('Name'),

        // The paths which the pod will display on.
        t('Paths (URLs)'),

        // The pod's weight column.
        t('Weight'),

        // The pod's status column.
        t('Status'),

        // The pod's action links, if the user has access to make changes.
        t('Actions'),
      ],
      '#rows' => [],
    ];

    // Loop through all pods and add them in to the table.
    foreach ($all_pods as $pod_machine_name => $pod) {
      // Build the pod's action links.
      $pod_action_links = [];

      // If the user has access to edit the pod, add an edit link.
      if (fcl_fcuk_communication_pods_access(COMMUNICATION_POD_ACTION_UPDATE, $pod)) {
        $pod_action_links[] = l(
          t(fcl_fcuk_communication_pods_get_action(COMMUNICATION_POD_ACTION_UPDATE, $pod)),
          COMMUNICATION_POD_ADMIN_URL . '/' . COMMUNICATION_POD_ACTION_UPDATE . '/' . $pod_machine_name);
      }

      // If the pod is in the database, it can be deleted/reverted if the user
      // has access.
      if (fcl_fcuk_communication_pods_pod_is_in_db($pod)) {
        if (fcl_fcuk_communication_pods_access(COMMUNICATION_POD_ACTION_DELETE, $pod)) {
          $pod_action_links[] = l(
            t(fcl_fcuk_communication_pods_get_action(COMMUNICATION_POD_ACTION_DELETE, $pod)),
            COMMUNICATION_POD_ADMIN_URL . '/' . COMMUNICATION_POD_ACTION_DELETE . '/' . $pod_machine_name);
        }
      }

      // Create the row.
      // @todo: This really ought to go in a theme hook, but would the time
      // to build be worth the effort.
      $render['pods_table']['#rows'][] = [
        'data' => [
          // The pod's name.
          '<h5 class="pod-friendly-name">' . check_plain($pod->{COMMUNICATION_POD_FRIENDLY_NAME}) . '</h5>'
          . '<p class="pod-machine-name">' . check_plain($pod->{COMMUNICATION_POD_MACHINE_NAME}) . '</p>',

          // The pod's paths.
          '<ul class="pod-paths"><li>' . implode('</li> <li>', $pod->paths) . '</li></ul>',

          // The pod's weight.
          '<p class="pod-weight">' . $pod->weight . '</p>',

          // The pod's status.
          '<div class="pod-status">'
          . ($pod->disabled ? t('disabled') : t('enabled')) . ' | '
          . fcl_fcuk_communication_pods_pod_status($pod_machine_name)
          . '</div>',

          // The pod's action links.
          '<div class="pod-action-links">' . implode('&nbsp;|&nbsp;', $pod_action_links) . '</div>',
        ],
      ];
    }
  }
  else {
    // No pods; show a "No pods" message with the option to create, if the user
    // has access.
    $render['no_pods_jim'] = [
      '#type' => 'html_tag',
      '#tag' => 'p',
      '#value' => t('<em>There are no Communication Pods to show at the moment.</em>'),
    ];
  }

  return render($render);
}

/**
 * Converts a string into a machine name for a Communication Pod.
 *
 * @param string $friendly_name
 *   The friendly name for the pod.
 *
 * @return string
 *   The friendly name converted to a machine name; lower-case, hyphens and
 *   spaces converted to underscores, and non-alphanumerics removed.
 *
 * @see fcl_fcuk_communication_pods_pod_machine_name_validate().
 */
function fcl_fcuk_communication_pods_convert_to_pod_machine_name($friendly_name) {
  // Convert the name to lower-case.
  $friendly_name = drupal_strtolower($friendly_name);

  // Convert spaces and hyphens to underscores.
  $friendly_name = str_replace([' ', '-'], '_', $friendly_name);

  // Remove any non-alphanumerics.
  $friendly_name = preg_replace("/[^a-z0-9_]/", '', $friendly_name);

  // Return it.
  return $friendly_name;
}

/**
 * Get the list of allowed HTML tags for a Communication Pod.
 *
 * @return array
 *   An array of HTML tags allowed.
 */
function fcl_fcuk_communication_pods_pod_html_allowed_tags() {
  $tags = [
    'div',
    'p',
    'h1',
    'h2',
    'h3',
    'h4',
    'h5',
    'h6',
    'span',
    'a',
    'em',
    'strong',
    'cite',
    'blockquote',
    'code',
    'ul',
    'ol',
    'li',
    'dl',
    'dt',
    'dd',
  ];

  // Implement a hook_fcl_fcuk_communication_pods_pod_html_allowed_tags_alter.
  drupal_alter('fcl_fcuk_communication_pods_pod_html_allowed_tags', $tags);

  return $tags;
}

/**
 * Determines whether a user has access to perform an action on a pod.
 *
 * @param string $action
 *   The action being performed; this is one of:
 *   - COMMUNICATION_POD_ACTION_CREATE
 *   - COMMUNICATION_POD_ACTION_READ (always granted).
 *   - COMMUNICATION_POD_ACTION_UPDATE
 *   - COMMUNICATION_POD_ACTION_DELETE
 *   - COMMUNICATION_POD_ACTION_REVERT (synonymous with delete; provided for
 *     completeness).
 *
 * @param CommunicationPod|null $pod
 *   The communication pod being checked, if the operation is "read", "update",
 *   "delete" or "revert"; if the operation is "create", then this should be
 *   NULL.
 *
 * @param User object|null $account
 *   Optional, a user object representing the user for whom the operation is to
 *   be performed. Determines access for a user other than the current user.
 *
 * @return bool
 *   COMMUNICATION_POD_ACCESS_GRANTED if allowed;
 *   COMMUNICATION_POD_ACCESS_DENIED if not.
 *
 * @see node_access().
 */
function fcl_fcuk_communication_pods_access($action, $pod = NULL, $account = NULL) {
  // If no user object is supplied, the access check is for the current user.
  if (empty($account)) {
    $account = $GLOBALS['user'];
  }

  // Initialise the grant variable. Default to access denied.
  $grant = COMMUNICATION_POD_ACCESS_DENIED;

  // If the action is read, we always grant permission.
  $grant = $grant || ($action === COMMUNICATION_POD_ACTION_READ);

  // If the user has site admin permissions, grant permission.
  // @todo: is this necessary/a good idea?
//  $grant = $grant || user_access('administer site configuration', $account);

  // If $pod is NULL and the op is create, check the user
  // access permissions.
  if (is_null($pod) && ($action === COMMUNICATION_POD_ACTION_CREATE)) {
    $grant = $grant || user_access(COMMUNICATION_POD_PERM_CREATE, $account);
  }

  // If $pod is not NULL, check if the user is the creator of the
  // pod.
  $pod_creator_uid = !empty($pod->uid) ? $pod->uid : NULL;

  // If $account->uid matches $pod_creator_uid and is not NULL or 0 (so we
  // don't grant access to anonymous users for file-based Communication Pods),
  // we can check if the user has permission to perform the action on their
  // own pods.
  if ($pod_creator_uid && ($pod_creator_uid == $account->uid)) {
    // Note that we only return COMMUNICATION_POD_ACCESS_GRANTED if the "own"
    // permissions checked here are TRUE; these are explicit grants, not
    // explicit denies.
    switch ($action) {
      // Check if the user can edit their own pods.
      case COMMUNICATION_POD_ACTION_UPDATE:
      case COMMUNICATION_POD_ACTION_OVERRIDE:
        $grant = $grant || user_access(COMMUNICATION_POD_PERM_EDIT_OWN, $account);
        break;

      // Check if the user can delete their own pods.
      case COMMUNICATION_POD_ACTION_DELETE:
      case COMMUNICATION_POD_ACTION_REVERT:
        $grant = $grant || user_access(COMMUNICATION_POD_PERM_DELETE_OWN, $account);
        break;
    }
  }

  // Lastly, check if the user has access to perform action on all pods.
  // Again, we only return COMMUNICATION_POD_ACCESS_GRANTED if the "own"
  // permissions checked here are TRUE; these are explicit grants, not
  // explicit denies.
  switch ($action) {
    // Check if the user can edit any pod.
    case COMMUNICATION_POD_ACTION_UPDATE:
    case COMMUNICATION_POD_ACTION_OVERRIDE:
      $grant = $grant || user_access(COMMUNICATION_POD_PERM_EDIT_ANY, $account);
      break;

    // Check if the user can delete any pod.
    case COMMUNICATION_POD_ACTION_DELETE:
    case COMMUNICATION_POD_ACTION_REVERT:
      $grant = $grant || user_access(COMMUNICATION_POD_PERM_DELETE_ANY, $account);
      break;
  }

  // Return the grant.
  return $grant;
}

/**
 * Returns the URI of a Communication Pod, per callback_entity_info_uri().
 *
 * @see https://api.drupal.org/api/drupal/modules%21system%21system.api.php/function/callback_entity_info_uri/7.x
 *
 * @param CommunicationPod $pod
 *   The Communication Pod which we want the URI for.
 *
 * @return array
 *   An associative array with the following elements:
 *   - 'path': The URL path for the entity.
 *   - 'options': (optional) An array of options for the url() function.
 *
 *   The actual entity URI can be constructed by passing these elements to
 *   url().
 */
function fcl_fcuk_communication_pods_pod_uri($pod) {
  return array(
    'path' => COMMUNICATION_POD_ENTITY_PATH_SLUG . '/' . $pod->{COMMUNICATION_POD_MACHINE_NAME},
  );
}

/**
 * Label callback to get a Communication Pod's label.
 *
 * @param entity $pod
 *   The Communication Pod which we want the URI for.
 *
 * @return string
 *   The Communication Pod's friendly name.
 */
function fcl_fcuk_communication_pods_pod_label($pod) {
  return $pod->{COMMUNICATION_POD_FRIENDLY_NAME};
}

/**
 * Page callback to view a Communication Pod.
 *
 * @param Communication Pod entity $pod
 *
 * @param string $view_mode
 *
 * @return string
 *   The render array for a Communication Pod.
 */
function fcl_fcuk_communication_pods_view_pod($pod, $view_mode = 'full') {
  $pod->content = array();

  // Build fields content.
  field_attach_prepare_view(COMMUNICATION_POD_ENTITY_NAME, [
    $pod->{COMMUNICATION_POD_MACHINE_NAME} => $pod,
  ], $view_mode);

  entity_prepare_view(COMMUNICATION_POD_ENTITY_NAME, [
    $pod->{COMMUNICATION_POD_MACHINE_NAME} => $pod,
  ]);

  $pod->content += field_attach_view(COMMUNICATION_POD_ENTITY_NAME, $pod, $view_mode);

  return $pod->content;
}

/**
 * Extracts the Communication Pod values to be saved from an edit form.
 *
 * @param array $form_state_values
 *   The $form_state['values'] array.
 *
 * @return object
 *   A CommunicationPod, ready to be saved.
 */
function fcl_fcuk_communication_pods_extract_edit_form_values(array $form_state_values) {
  // Get the pod entity from the server-side field we saved it into.
  $pod = $form_state_values[COMMUNICATION_POD_ENTITY_NAME];

  $pod->uid = $form_state_values['uid'];
  $pod->{COMMUNICATION_POD_MACHINE_NAME} = $form_state_values[COMMUNICATION_POD_MACHINE_NAME];
  $pod->{COMMUNICATION_POD_FRIENDLY_NAME} = $form_state_values[COMMUNICATION_POD_FRIENDLY_NAME];
  $pod->pod_html = $form_state_values['pod_html'];
  $pod->pod_css = $form_state_values['pod_css'];
  $pod->pod_external_css = fcl_uk_utilities_text_field_to_array($form_state_values['pod_external_css']);
  $pod->pod_js = $form_state_values['pod_js'];
  $pod->pod_external_js = fcl_uk_utilities_text_field_to_array($form_state_values['pod_external_js']);
  $pod->paths = fcl_uk_utilities_text_field_to_array($form_state_values['paths']);
  $pod->weight = $form_state_values['weight'];
  $pod->disabled = $form_state_values['disabled'];

  return $pod;
}

/**
 * Converts a Communication Pod entity ID to a machine name.
 *
 * @param int $entity_id
 *   The entity ID.
 *
 * @return string|false
 *   The machine name of the entity, if found, or FALSE if not.
 */
function fcl_fcuk_communication_pods_entity_id_to_machine_name($entity_id) {
  return entity_load(COMMUNICATION_POD_ENTITY_NAME, [$entity_id])->{COMMUNICATION_POD_MACHINE_NAME};
}

/**
 * Converts a Communication Pod machine name to an entity ID.
 *
 * @param string|array $pod_machine_names
 *   A single Communication Pod's machine name as a string, or an array of
 *   names. Yay for flexibility!
 *
 * @param bool $include_empty_responses
 *   For unknown entities, do we want to return an array key with FALSE to
 *   indicate we looked but it wasn't found, or simply not include an array
 *   key at all.
 *
 * @return array|bool|int
 * If a single machine name was passed in as a string, the Communication
 * Pod's entity ID, if found, or FALSE if not. If an array of pod machine
 * names was passed in, this function will return an array whose keys are
 * the pods' machine names, and the values are either the pod's entity ID,
 * if found, or FALSE if not.
 */
function fcl_fcuk_communication_pods_machine_name_to_entity_id($pod_machine_names, $include_empty_responses = FALSE) {
  $pod_machine_names_loaded = &drupal_static(__FUNCTION__, []);

  // Keep track of whether we've been asked for a single entity ID, or
  // several.
  $multiple = is_array($pod_machine_names);

  // Make sure machine names is an array.
  $pod_machine_names = (array) $pod_machine_names;

  foreach ($pod_machine_names as $pod_machine_name) {
    // Have we already been asked for this ID?
    if (!array_key_exists($pod_machine_name, $pod_machine_names_loaded)) {
      $pod_machine_names_loaded[$pod_machine_name] = FALSE;

      if ($pod = fcl_fcuk_communication_pods_load_from_db($pod_machine_name, [COMMUNICATION_POD_ID_FIELD])) {
        $pod_machine_names_loaded[$pod_machine_name] = $pod->{COMMUNICATION_POD_ID_FIELD};
      }
    }
  }

  // Return the ID(s).
  if ($multiple) {
    $pod_ids = [];

    foreach ($pod_machine_names as $pod_machine_name) {
      // Only add the entity's information if it was found or it wasn't found
      // and $include_empty_responses is TRUE.
      $pod_id = $pod_machine_names_loaded[$pod_machine_name];

      if ($pod_id || $include_empty_responses) {
        $pod_ids[$pod_machine_name] = $pod_id;
      }
    }

    return $pod_ids;
  }
  else {
    // We don't need to worry about $include_empty_responses here because
    // an unknown entity means we would just return FALSE in any case.
    $pod_machine_name = reset($pod_machine_names);
    return $pod_machine_names_loaded[$pod_machine_name];
  }
}

/**
 * Loads a single communication pod record from the database by machine name.
 *
 * @param string $pod_machine_name
 *   The Communication Pod's machine name.
 *
 * @param array $fields_to_load
 *   An array of field names to load. If not provided, all fields will be
 *   loaded.
 *
 * @return false|object
 * The Communication Pod's database record, if found, or FALSE if not.
 */
function fcl_fcuk_communication_pods_load_from_db($pod_machine_name, array $fields_to_load = []) {
  $loaded_pods = &drupal_static(__FUNCTION__, []);

  // Create a cache ID which represents the pod machine name and the keys
  // requested.
  $cache_id = $pod_machine_name . '::' . implode(':', $fields_to_load);

  // If we haven't already checked this pod, attempt to load it from the
  // database.
  if (!array_key_exists($cache_id, $loaded_pods)) {
    $loaded_pods[$cache_id] = db_select(COMMUNICATION_POD_SCHEMA_NAME, COMMUNICATION_POD_SCHEMA_NAME)
      ->fields(COMMUNICATION_POD_SCHEMA_NAME, $fields_to_load)
      ->condition(COMMUNICATION_POD_MACHINE_NAME, $pod_machine_name)
      ->execute()
      ->fetchObject();
  }

  return $loaded_pods[$cache_id];
}

/**
 * Fleshes out a skeleton Communication Pod.
 *
 * @param CommunicationPod object $pod
 */
function _fcl_fcuk_communication_pods_decorate_empty_pod(stdClass &$pod) {
  $fields_to_add = [
    COMMUNICATION_POD_MACHINE_NAME => '',
    COMMUNICATION_POD_FRIENDLY_NAME => '',
    'uid' => NULL,
    'pod_html' => '<h3>Example communication pod</h3>
<p>Add your HTML here :)</p>',
    'pod_css' => '',
    'pod_external_css' => [],
    'pod_js' => '/**
 * @file
 * Javascript for this communication pod.
 */
(function($) {
  \'use strict\';
  // Example function.
  Drupal.behaviors.fclUkCommunicationPodExample = {
    attach: function(context, settings) {
      // Do something.
    }
  };
})(jQuery);
',
    'pod_external_js' => [],
    'paths' => ['*'],
    'disabled' => FALSE,
    'weight' => 0,
    'language' => LANGUAGE_NONE,
    'extra_information' => [],
  ];

  // Add all the values to the pod, but only if they aren't already set.
  foreach ($fields_to_add as $key => $value) {
    if (empty($pod->{$key})) {
      $pod->{$key} = $value;
    }
  }
}

/**
 * (Un)serialise an object's values which are objects or arrays.
 *
 * @param CommunicationPod entity $pod
 * @param string $action
 *   The action to take; either 'serialize' or 'unserialize'. Note the
 *   American spelling. U-S-A! U-S-A!
 */
function fcl_fcuk_communication_pods_serialize_unserialize_values($pod, $action) {
  foreach ($pod as $key => &$value) {
    // Only attempt to unserialize if it's serialised.
    if ($action == 'unserialize') {
      // Check if unserialising works.
      $unserialized_value = @unserialize($value);
      if ($unserialized_value !== FALSE) {
        $value = $unserialized_value;
      }
    }
    elseif ($action == 'serialize') {
      if (is_array($value) || is_object($value)) {
        $value = serialize($value);
      }
    }
  }
}

/**
 * Gets a render array for a Communication Pod.
 *
 * @param CommunicationPod entity $pod
 *
 * @return array|bool
 */
function fcl_fcuk_communication_pods_get_render_array($pod) {
  // Make sure this is a valid communication pod.
  if (fcl_fcuk_communication_pods_valid_pod($pod)) {
    return [
      '#' . COMMUNICATION_POD_MACHINE_NAME => $pod->{COMMUNICATION_POD_MACHINE_NAME},
      '#theme' => COMMUNICATION_POD_THEME_FUNCTION,
      '#attached' => [
        // Caveat developer: you can't safely use #attached to attach JS or CSS
        // to a Communication Pod because, if it's loaded over Ajax, I don't
        // think it will be added, so you need to add the CSS file to the
        // chunk of HTML delivered as part of the pod.
//        'css' => [],
      ],
    ];
  }

  return FALSE;
}

/**
 * Check if a Communication Pod appears to be valid.
 *
 * @param CommunicationPod entity $pod
 *
 * @return bool
 *   TRUE if the pod looks valid; FALSE if not.
 */
function fcl_fcuk_communication_pods_valid_pod($pod) {
  return !empty($pod) && is_object($pod) && !empty($pod->{COMMUNICATION_POD_MACHINE_NAME});
}
