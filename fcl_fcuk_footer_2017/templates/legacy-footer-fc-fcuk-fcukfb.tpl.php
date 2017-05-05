<?php
/**
 * @file
 * The legacy footer code which was used on the fc, fcuk and fcukfb themes.
 */
?>

<footer id="footer">
  <div id="footer-inner">

    <div class="footer-content">
      <?php if ($fc_footer_content): ?>
        <?php print $fc_footer_content; ?>
      <?php endif; ?>

      <?php if ($fc_lag): ?>
        <?php print $fc_lag; ?>
      <?php endif; ?>
    </div>

    <?php if ($fc_lag_link): ?>
      <a class="footer-lag tablet-hide" href="<?php print $fc_lag_link; ?>"></a>
    <?php endif; ?>

    <?php if ($fc_footer_links): ?>
      <div class="footer-links">
        <?php print $fc_footer_links; ?>
        <?php print $fc_footer_links_two; ?>
      </div>
    <?php endif; ?>

  </div><!-- #footer-inner -->

  <?php if ($footer): ?>
    <?php print $footer; ?>
  <?php endif; ?>

  <?php if ($fc_copyright || $fc_brand_links): ?>
    <div id="footer-privacy">
      <div id="footer-privacy-inner">
        <?php if ($fc_copyright): ?>
          <p class="footer-terms"><?php print $fc_copyright; ?>
            <?php if ($fc_copyright): ?>
              <br/><?php print $fc_licence; ?>
            <?php endif; ?>
          </p>
        <?php endif; ?>

        <?php if ($fc_brand_links): ?>
          <div class="footer-site-links mobile-hide">
            <?php print $fc_brand_links; ?>
          </div>
        <?php endif; ?>
      </div><!-- #footer-privacy-inner -->
    </div><!-- #footer-privacy -->
  <?php endif; ?>

  <?php
  $footer_bottom = render($page['footer_bottom']);
  ?>

  <?php if ($footer_bottom || $fc_facebook || $fc_twitter || $fc_google || $fc_blog || $fc_subscribe || $fc_instagram): ?>

    <div id="footer-bottom">
      <div id="footer-bottom-inner">
        <?php if ($fc_facebook || $fc_twitter || $fc_google || $fc_blog || $fc_subscribe || $fc_instagram): ?>
          <ul class="footer-social">
            <?php if ($fc_facebook): ?>
              <li><a href="<?php print $fc_facebook; ?>" class="facebook" target="_blank">
                  <span class="icon"></span>
                  <span>Join on Facebook</span>
                </a></li>
            <?php endif; ?>
            <?php if ($fc_twitter): ?>
              <li><a href="<?php print $fc_twitter; ?>" class="twitter" target="_blank">
                  <span class="icon"></span>
                  <span>Follow on Twitter</span>
                </a></li>
            <?php endif; ?>
            <?php if ($fc_google): ?>
              <li><a href="<?php print $fc_google; ?>" class="google-plus" target="_blank">
                  <span class="icon"></span>
                  <span>Add on Google +</span>
                </a></li>
            <?php endif; ?>
            <?php if ($fc_instagram): ?>
              <li><a href="<?php print $fc_instagram; ?>" class="instagram" target="_blank">
                  <span class="icon"></span>
                  <span>Instagram</span>
                </a></li>
            <?php endif; ?>
            <?php if ($fc_blog): ?>
              <li><a href="<?php print $fc_blog; ?>" class="travel-blog" target="_blank">
                  <span class="icon"></span>
                  <span>Travel Blog</span>
                </a></li>
            <?php endif; ?>
            <?php if ($fc_subscribe): ?>
              <li class="last"><a href="<?php print $fc_subscribe; ?>" class="subscribe" target="_blank">
                  <span class="icon"></span>
                  <span>Subscribe for great deals</span>
                </a></li>
            <?php endif; ?>
          </ul>
        <?php endif; ?>

        <?php print $footer_bottom; ?>

      </div>
    </div>

  <?php endif; ?>

  <?php if (!empty($fc_footer_copyright)): ?>
    <?php print render($fc_footer_copyright); ?>
  <?php endif; ?>

</footer><!-- region__footer -->
