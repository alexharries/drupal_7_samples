<?php
/**
 * @file
 * The legacy footer used on the FCL UK FC theme, which is still used on FCIE.
 */
?>

<footer id="footer">
  <?php fcl_uk_utilities_print($content['footer_top'], '<div class="footer-top" id="footer-top"><div class="container">', '</div></div>'); ?>
  <?php fcl_uk_utilities_print($content['footer'], '<div class="footer-middle" id="footer-middle"><div class="container">', '</div></div>'); ?>
  <?php fcl_uk_utilities_print($content['footer_bottom'], '<div class="footer-bottom" id="footer-bottom"><div class="container">', '</div></div>'); ?>
</footer>

