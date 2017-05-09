<?php

/**
 * @file
 * Help text for the Communication Pods module.
 */

/**
 * Simple function to get Communication Pods help text.
 *
 * @todo: This could probably be more configuraboo/less clunky.
 *
 * @param string $what_help_do_you_require
 *   The help text requested.
 *
 * @return string
 *   A chunk of HTML help text, or an empty string if $what_help_do_you_require
 *   didn't make any sense.
 */
function fcl_fcuk_communication_pods_help($what_help_do_you_require) {
  $help_html = '';

  switch ($what_help_do_you_require) {
    // This help is shown on the create/edit pod page.
    case 'help_text_creating_pods':
      $help_html = '<em>' . t('To add a Communication Pod to the website, follow the instructions on this page to add the HTML, CSS and any JavaScript for your new pod. Be careful to set it to only show on the URLs you need it to appear. Then, you need to add a Communication Pod spot to a page; the easiest way to do this is to add a Communication Pod paragraph to a Ribbons Page - you can <a href="@add_ribbons_page_url" target="_blank">add a Ribbons page here</a>.', [
          '@add_ribbons_page_url' => url('node/add/uk-page'),
        ]) . '</em>';

      break;

    // This help is shown on the pods overview page.
    case 'help_text_pods_overview':
      $help_html = '<p><em>' . t('Communication Pods are little self-contained chunks of HTML which you can set to display on selected pages around the site. You can upload CSS (styles) and Javascript, or you can link to external styles and Javascript files.') . '</em></p>';

      $help_html .= '<p><em>' . t('Once you have configured a Communication Pod to display on a particular path (URL) on the site, you need to add a place for the pod to be displayed; for example, if the path is "hello/world", you can <a href="@add_ribbons_page_url" target="_blank">add a Ribbons page</a> at that path and add a Communication Pod ribbon to the page which will display your new pod.', [
          '@add_ribbons_page_url' => url('node/add/uk-page'),
        ]) . '</em></p>';

      break;
  }

  return $help_html;
}
