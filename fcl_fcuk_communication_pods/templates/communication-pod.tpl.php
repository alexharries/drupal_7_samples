<?php

/**
 * @file
 * Creates a communication pod.
 */

?>

<div class="communication-pod-wrapper <?php print $wrapper_classes ?>">
  <?php if (!empty($pod_css)): ?>
    <!-- Pod CSS. -->
    <style type="text/css"><?php print $pod_css; ?></style>
  <?php endif; ?>

  <?php if (!empty($pod_external_css)): ?>
    <!-- External CSS files. -->
    <?php foreach ($pod_external_css as $pod_external_css_file_url): ?>
      <link type="text/css" rel="stylesheet" href="<?php print $pod_external_css_file_url; ?>" media="all"/>
    <?php endforeach; ?>
  <?php endif; ?>

  <?php if (!empty($pod_js)): ?>
    <!-- Pod JS. -->
    <script type="text/javascript"><?php print $pod_js; ?></script>
  <?php endif; ?>

  <?php if (!empty($pod_external_js)): ?>
    <!-- External CSS files. -->
    <?php foreach ($pod_external_js as $pod_external_js_file_url): ?>
      <script type="text/javascript" src="<?php print $pod_external_js_file_url; ?>"></script>
    <?php endforeach; ?>
  <?php endif; ?>

  <div class="<?php print $classes; ?>">
    <?php print render($pod_html); ?>

    <?php if (!empty($edit_link) || !empty($delete_link)): ?>
      <div class="actions-wrapper">
        <?php if (!empty($edit_link)): ?>
          <?php print render($edit_link); ?>
        <?php endif; ?>

        <?php if (!empty($edit_link) && !empty($delete_link)): ?>
          |
        <?php endif; ?>

        <?php if (!empty($delete_link)): ?>
          <?php print render($delete_link); ?>
        <?php endif; ?>
      </div>
    <?php endif; ?>
  </div>
</div>
