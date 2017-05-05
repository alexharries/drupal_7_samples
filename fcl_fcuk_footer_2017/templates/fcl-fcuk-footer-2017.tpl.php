<?php
/**
 * @file
 * Template to render the FCUK page footer.
 *
 * @todo: consider moving all the components out of this file and into their
 *      own theme callbacks, e.g. social links.
 */
?>

<footer id="footer" class="variant-<?php print $variant; ?><?php if (isset($_GET['gru'])): ?> gru<?php endif; ?>">
  <div class="footer-top-section">
    <div class="container">
      <!-- First row - "need more help?" and award logos. -->
      <div class="row clearfix">
        <div class="col-md-6 need-more-help-contact-an-expert">
          <h2 class="need-more-help-title"><?php print t('Need more help?'); ?></h2>
          <div class="contact-an-expert">
            <?php print render($contact_an_expert); ?>
          </div>
        </div>

        <div class="col-sm-8 col-sm-offset-2 col-md-5 col-md-offset-1 top-award-logos">
          <div class="top-award-logo winner-british-travel-awards-2016">
            <img src="<?php print $module_url ?>/images/winner-british-travel-awards-2016-variant-<?php print $variant ?>.png"
              alt="<?php print t('Winner - British Travel Awards 2016 - Best Regional Travel Agency'); ?>"
              title="<?php print t('Winner - British Travel Awards 2016 - Best Regional Travel Agency'); ?>"/>
          </div>

          <div class="top-award-logo number-1-to-australia">
            <h3>
              <span class="number-1-to-australia-strapline"><?php print t('Number One to Australia'); ?></span>
              <img src="<?php print $module_url ?>/images/worlds-number-1-to-australia-variant-<?php print $variant ?>.png"
                alt="<?php print t('Number One to Australia'); ?>"
                title="<?php print t('Number One to Australia'); ?>"/>
            </h3>
          </div>

          <div class="top-award-logo good-housekeeping-reader-recommended">
            <img src="<?php print $module_url ?>/images/good-housekeeping-reader-recommended-variant-<?php print $variant ?>.png"
              alt="<?php print t('Good Housekeeping Reader Recommended 2016'); ?>"
              title="<?php print t('Good Housekeeping Reader Recommended 2016'); ?>"/>
          </div>
        </div>
      </div>

      <!-- Second row - quick links menus on desktop, 5 columns. -->
      <div class="row mobile-hide tablet-hide clearfix">
        <?php
        // Note that we repeat the footer links so we can show the links
        // above the newsletter signup and social icons on desktop, and below
        // them on mobile.
        // @todo: move this into a separate theme function so we can keep this
        // code DRY?
        ?>
        <div class="footer-links col-md-12">
          <div class="footer-links-list footer-links-list-1">
            <h4><?php print t('Quick Links'); ?></h4>
            <?php print drupal_render($menu_quick_links_desktop) ?>
          </div>

          <div class="footer-links-list footer-links-list-2">
            <h4><?php print t('Flights'); ?></h4>
            <?php print drupal_render($menu_flights_desktop) ?>
          </div>

          <div class="footer-links-list footer-links-list-3">
            <h4><?php print t('Holidays'); ?></h4>
            <?php print drupal_render($menu_holidays_desktop) ?>
          </div>

          <div class="footer-links-list footer-links-list-4">
            <h4><?php print t('About Flight Centre'); ?></h4>
            <?php print drupal_render($menu_about_desktop) ?>
          </div>

          <div class="footer-links-list footer-links-list-5">
            <h4><?php print t('Contact us'); ?></h4>
            <?php print drupal_render($menu_contact_desktop) ?>
          </div>
        </div>
      </div>

      <!-- Third row - newsletter subscription and social links. -->
      <div class="row clearfix">
        <div class="col-md-push-7 col-md-2 col-md-offset-3 social-links">
          <ul>
            <li class="twitter">
              <a href="https://twitter.com/flightcentreUK" title="<?php print t('@twitter_username on Twitter', ['@twitter_username' => '@flightcentreUK']) ?>">
                <svg id="footer-twitter" xmlns="http://www.w3.org/2000/svg" width="36" height="36" viewBox="0 0 400 400">
                  <defs>
                    <style>
                      #footer-twitter .cls-1 {
                        fill: #1da1f2;
                      }

                      #footer-twitter .cls-2 {
                        fill: #fff;
                        fill-rule: evenodd;
                      }
                    </style>
                  </defs>
                  <rect id="Dark_Blue" data-name="Dark Blue" class="cls-1" width="400" height="400" rx="30" ry="30"/>
                  <path id="Logo_FIXED" data-name="Logo — FIXED" class="cls-2" d="M153.623,302c94.344,0,145.936-78.093,145.936-145.809,0-2.219-.045-4.428-0.147-6.625A104.324,104.324,0,0,0,325,123.032a102.442,102.442,0,0,1-29.455,8.068A51.427,51.427,0,0,0,318.1,102.752a102.841,102.841,0,0,1-32.568,12.439,51.322,51.322,0,0,0-87.408,46.736A145.659,145.659,0,0,1,92.4,108.385,51.257,51.257,0,0,0,108.278,176.8a50.942,50.942,0,0,1-23.23-6.41c-0.011.214-.011,0.428-0.011,0.653a51.28,51.28,0,0,0,41.15,50.241,51.23,51.23,0,0,1-23.163.878,51.344,51.344,0,0,0,47.916,35.594,102.976,102.976,0,0,1-63.7,21.937A104.315,104.315,0,0,1,75,278.982,145.3,145.3,0,0,0,153.623,302"/>
                </svg>
              </a>
            </li>

            <li class="facebook">
              <a href="https://www.facebook.com/FlightCentreUK" title="<?php print t('/@facebook_pagename on Facebook') ?>">
                <svg id="footer-facebook" xmlns="http://www.w3.org/2000/svg" width="36" height="36" viewBox="0 0 2134 2135">
                  <defs>
                    <style>
                      #footer-facebook .cls-1 {
                        fill: #3664a2;
                      }

                      #footer-facebook .cls-1,
                      #footer-facebook .cls-2 {
                        fill-rule: evenodd;
                      }

                      #footer-facebook .cls-2 {
                        fill: #fff;
                      }
                    </style>
                  </defs>
                  <path id="Blue-2" data-name="Blue" class="cls-1" d="M2015.93,2134.26a117.647,117.647,0,0,0,117.68-117.69V118.435A117.641,117.641,0,0,0,2015.93.739H118.074A117.587,117.587,0,0,0,.438,118.435V2016.57a117.593,117.593,0,0,0,117.636,117.69H2015.93Z"/>
                  <path id="Blue-3" data-name="Blue" class="cls-2" d="M1473.21,2134.26V1307.09H1749.6l41.52-320.906H1473.21V780.464c0-93.231,24.97-156.765,158.65-156.765l170.49,0.053V335.618c-29.5-3.919-130.7-11.9-248.44-11.9-245.81,0-413.96,149.281-413.96,424.867v237.6H860.759V1307.09H1139.88v827.17h333.33Z"/>
                </svg>
              </a>
            </li>

            <li class="instagram">
              <a href="https://instagram.com/flightcentreuk" title="<?php print t('@instagram_username on Instagram', ['@instagram_username' => 'flightcentreuk']) ?>">
                <svg id="instagran" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 502.68 502.68">
                  <defs>
                    <style>
                      #instagran .cls-1 {
                        fill: #bc2a8d;
                      }

                      #instagran .cls-2 {
                        fill: #fff;
                      }
                    </style>
                  </defs>
                  <title>instagran</title>
                  <rect class="cls-1" width="502.68" height="502.68" rx="72" ry="72"/>
                  <path class="cls-2"
                    d="M250.4,78.07c57.2,0,64,.22,86.56,1.25,20.89,1,32.23,4.44,39.78,7.38a70.94,70.94,0,0,1,40.66,40.66c2.93,7.55,6.42,18.89,7.38,39.78,1,22.59,1.25,29.36,1.25,86.56s-.22,64-1.25,86.56c-1,20.89-4.44,32.23-7.38,39.78a70.94,70.94,0,0,1-40.66,40.66c-7.55,2.93-18.89,6.42-39.78,7.38-22.59,1-29.36,1.25-86.56,1.25s-64-.22-86.56-1.25c-20.89-1-32.23-4.44-39.78-7.38A70.94,70.94,0,0,1,83.4,380C80.46,372.49,77,361.15,76,340.26c-1-22.59-1.25-29.36-1.25-86.56s.22-64,1.25-86.56c1-20.89,4.44-32.23,7.38-39.78A70.94,70.94,0,0,1,124.05,86.7c7.55-2.93,18.89-6.42,39.78-7.38,22.59-1,29.36-1.25,86.56-1.25m0-38.6c-58.18,0-65.47.25-88.32,1.29s-38.37,4.66-52,10a109.54,109.54,0,0,0-62.65,62.65c-5.3,13.63-8.92,29.2-10,52s-1.29,30.14-1.29,88.32.25,65.47,1.29,88.32,4.66,38.37,10,52a109.54,109.54,0,0,0,62.65,62.65c13.63,5.3,29.2,8.92,52,10s30.14,1.29,88.32,1.29,65.47-.25,88.32-1.29,38.37-4.66,52-10A109.54,109.54,0,0,0,453.38,394c5.3-13.63,8.92-29.2,10-52s1.29-30.14,1.29-88.32-.25-65.47-1.29-88.32-4.66-38.37-10-52a109.54,109.54,0,0,0-62.65-62.65c-13.63-5.3-29.2-8.92-52-10s-30.14-1.29-88.32-1.29Z"/>
                  <path class="cls-2"
                    d="M250.4,143.69a110,110,0,1,0,110,110A110,110,0,0,0,250.4,143.69Zm0,181.41a71.41,71.41,0,1,1,71.41-71.41A71.41,71.41,0,0,1,250.4,325.1Z"/>
                  <circle class="cls-2" cx="364.75" cy="139.34" r="25.71"/>
                </svg>
              </a>
            </li>
          </ul>
        </div>

        <div class="col-md-pull-5 col-md-7 newsletter-signup-form">
          <div class="static-form">
            <?php print render($newsletter_form) ?>
          </div>
        </div>
      </div>

      <!-- Fourth row - quick links menus on mobile/tablet, 5 columns. -->
      <div class="row desktop-hide clearfix">
        <?php
        // Note that we repeat the footer links so we can show the links
        // above the newsletter signup and social icons on desktop, and below
        // them on mobile.
        // @todo: move this into a separate theme function so we can keep this
        // code DRY?
        ?>
        <div class="footer-links col-md-12">
          <div class="footer-links-list footer-links-list-1 col-xs-6 col-smaller-3 col-smaller-offset-1">
            <h4><?php print t('Quick Links'); ?></h4>
            <?php print drupal_render($menu_quick_links_mobile) ?>
          </div>

          <div class="footer-links-list footer-links-list-2 col-xs-6 col-smaller-3">
            <h4><?php print t('Flights'); ?></h4>
            <?php print drupal_render($menu_flights_mobile) ?>
          </div>

          <div class="footer-links-list footer-links-list-3 col-xs-6 col-smaller-4">
            <h4><?php print t('Holidays'); ?></h4>
            <?php print drupal_render($menu_holidays_mobile) ?>
          </div>

          <div class="footer-links-list footer-links-list-4 col-xs-6 col-smaller-4 col-smaller-offset-1">
            <h4><?php print t('About Flight Centre'); ?></h4>
            <?php print drupal_render($menu_about_mobile) ?>
          </div>

          <div class="footer-links-list footer-links-list-5 col-xs-6 col-smaller-4">
            <h4><?php print t('Contact us'); ?></h4>
            <?php print drupal_render($menu_contact_mobile) ?>
          </div>
        </div>
      </div>
    </div>
  </div>

  <div class="footer-bottom-section">
    <div class="container">
      <!-- First row - more award logos and accreditations. -->
      <div class="row bottom-award-logos clearfix col-md-12">
        <div class="bottom-award-logos-inner">
          <div class="bottom-award-logo col-md-2-disabled col-xs-3-disabled travelife-award">
            <img src="<?php print $module_url ?>/images/travelife-variant-<?php print $variant ?>.png"
              alt="<?php print t('Travelife - Sustainability In Action - Tour Operators and Travel Agents'); ?>"
              title="<?php print t('Travelife - Sustainability In Action - Tour Operators and Travel Agents'); ?>"/>
          </div>

          <div class="bottom-award-logo col-md-2-disabled col-xs-3-disabled sunday-times-best-to-work-for">
            <img src="<?php print $module_url ?>/images/sunday-times-100-best-companies-variant-<?php print $variant ?>.svg"
              alt="<?php print t('The Sunday Times - 100 Best Companies To Work For'); ?>"
              title="<?php print t('The Sunday Times - 100 Best Companies To Work For'); ?>"/>
          </div>

          <div class="bottom-award-logo col-md-2-disabled col-xs-3-disabled iata-accredited">
            <img src="<?php print $module_url ?>/images/iata-accredited-agent-variant-<?php print $variant ?>.svg"
              alt="<?php print t('IATA Accredited Travel Agent'); ?>"
              title="<?php print t('IATA Accredited Travel Agent'); ?>"/>
          </div>

          <div class="bottom-award-logo col-md-2-disabled col-xs-3-disabled winner-british-travel-awards-2016">
            <img src="<?php print $module_url ?>/images/winner-british-travel-awards-2016-bottom-row-variant-<?php print $variant ?>.png"
              alt="<?php print t('Winner - British Travel Awards 2016 - Best Regional Travel Agency'); ?>"
              title="<?php print t('Winner - British Travel Awards 2016 - Best Regional Travel Agency'); ?>"/>
          </div>

          <div class="bottom-award-logo col-md-2-disabled col-xs-3-disabled abta">
            <img src="<?php print $module_url ?>/images/abta-variant-<?php print $variant ?>.svg"
              alt="<?php print t('ABTA - Travel With Confidence'); ?>"
              title="<?php print t('ABTA - Travel With Confidence'); ?>"/>
          </div>

          <div class="bottom-award-logo col-md-2-disabled col-xs-3-disabled atol-protected">
            <img src="<?php print $module_url ?>/images/atol-protected-variant-<?php print $variant ?>.svg"
              alt="<?php print t('ATOL Protected'); ?>"
              title="<?php print t('ATOL Protected'); ?>"/>
          </div>
        </div>
      </div>

      <!-- Second row - Copyright, company number, and phone calls information. -->
      <div class="row copyright-company-number-phone-info clearfix col-md-12">
        <p class="copyright-company-number">
          <?php print t('&copy; @year Flight Centre (UK) Limited, Registered in England No. 02937210', ['@year' => date('Y')]); ?>
        </p>

        <p class="phone-info">
          <?php print t('<superscript>*</superscript> 0800 calls are free for landlines and mobiles. 0333 calls are included within inclusive minutes package on mobiles, otherwise standard rates apply. 0844/0845 calls are 7p/pm plus your local carrier charge.'); ?>
        </p>
      </div>
    </div>
  </div>
</footer>
