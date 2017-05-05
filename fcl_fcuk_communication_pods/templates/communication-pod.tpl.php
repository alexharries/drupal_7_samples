<?php

/**
 * @file
 * Creates a communication pod.
 */

// Inject the links to any CSS and JS provided by the pod.
echo $pod_css;
echo $pod_js;
?>

<div class="<?php print $classes; ?>">
  <?php print $pod_html; ?>
</div>
