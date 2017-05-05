<?php

/**
 * @file
 * fcl_fcuk_footer_2017.footer_menu_links
 */

/**
 * Constants.
 */

// The menu names.
const FCL_FCUK_FOOTER_2017_MENU_QUICK_LINKS = 'fcl-fcuk-footer-quick-links';
const FCL_FCUK_FOOTER_2017_MENU_FLIGHTS = 'fcl-fcuk-footer-flights';
const FCL_FCUK_FOOTER_2017_MENU_HOLIDAYS = 'fcl-fcuk-footer-holidays';
const FCL_FCUK_FOOTER_2017_MENU_ABOUT = 'fcl-fcuk-footer-about';
const FCL_FCUK_FOOTER_2017_MENU_CONTACT = 'fcl-fcuk-footer-contact';

/**
 * Creates the footer menus using menu_save().
 *
 * @see menu_save().
 */
function fcl_fcuk_footer_2017_footer_menu_create() {
  $menus = [
    [
      'menu_name' => FCL_FCUK_FOOTER_2017_MENU_QUICK_LINKS,
      'title' => 'Flight Centre UK Footer Quick Links',
      'description' => "Contains quick links to the site's key content areas.",
    ],
    [
      'menu_name' => FCL_FCUK_FOOTER_2017_MENU_FLIGHTS,
      'title' => 'Flight Centre UK Footer Flights',
      'description' => 'Links to flights.',
    ],
    [
      'menu_name' => FCL_FCUK_FOOTER_2017_MENU_HOLIDAYS,
      'title' => 'Flight Centre UK Footer Holidays',
      'description' => 'Links to holidays.',
    ],
    [
      'menu_name' => FCL_FCUK_FOOTER_2017_MENU_ABOUT,
      'title' => 'Flight Centre UK Footer - About Flight Centre',
      'description' => 'Links to information about Flight Centre.',
    ],
    [
      'menu_name' => FCL_FCUK_FOOTER_2017_MENU_CONTACT,
      'title' => 'Flight Centre UK Footer Contact Information',
      'description' => 'Links to contact information.',
    ],
  ];

  foreach ($menus as $menu) {
    menu_save($menu);
  }
}

/**
 * Creates the footer menu links.
 */
function fcl_fcuk_footer_2017_footer_menu_links_create() {
  $menu_links_by_menu = [
    FCL_FCUK_FOOTER_2017_MENU_QUICK_LINKS => [
      [
        'link_title' => 'Flights',
        'link_path' => 'flights',
      ],
      [
        'link_title' => 'RTW Flights',
        'link_path' => 'flights/round-the-world-flights/round-the-world-tickets',
      ],
      [
        'link_title' => 'First and Business',
        'link_path' => 'first-and-business',
      ],
      [
        'link_title' => 'Holidays',
        'link_path' => 'holidays',
      ],
      [
        'link_title' => 'Tours',
        'link_path' => 'group-tours',
      ],
      [
        'link_title' => 'Business Travel',
        'link_path' => 'business-travel',
      ],
      [
        'link_title' => 'Travel Advice',
        'link_path' => 'before-you-fly',
      ],
      [
        'link_title' => 'Special Offers',
        'link_path' => 'offers',
      ],
      [
        'link_title' => 'Travel Blog',
        'link_path' => 'http://www.flightcentre.co.uk/uk-travel-blog',
      ],
    ],
    FCL_FCUK_FOOTER_2017_MENU_FLIGHTS => [
      [
        'link_title' => 'Asia',
        'link_path' => 'flights/asia',
      ],
      [
        'link_title' => 'Australia',
        'link_path' => 'flights/australia',
      ],
      [
        'link_title' => 'Canada',
        'link_path' => 'flights/canada',
      ],
      [
        'link_title' => 'Middle East',
        'link_path' => 'flights/middle-east',
      ],
      [
        'link_title' => 'New Zealand',
        'link_path' => 'flights/new-zealand',
      ],
      [
        'link_title' => 'South Africa',
        'link_path' => 'flights/south-africa',
      ],
      [
        'link_title' => 'USA',
        'link_path' => 'flights/united-states',
      ],
    ],
    FCL_FCUK_FOOTER_2017_MENU_HOLIDAYS => [
      [
        'link_title' => 'Asia',
        'link_path' => 'holidays/asia',
      ],
      [
        'link_title' => 'Australia',
        'link_path' => 'holidays/australia',
      ],
      [
        'link_title' => 'Canada',
        'link_path' => 'holidays/canada',
      ],
      [
        'link_title' => 'Middle East',
        'link_path' => 'holidays/middle-east',
      ],
      [
        'link_title' => 'New Zealand',
        'link_path' => 'holidays/new-zealand',
      ],
      [
        'link_title' => 'South Africa',
        'link_path' => 'holidays/south-africa',
      ],
      [
        'link_title' => 'USA',
        'link_path' => 'holidays/united-states',
      ],
    ],
    FCL_FCUK_FOOTER_2017_MENU_ABOUT => [
      [
        'link_title' => 'About us',
        'link_path' => 'about-us',
      ],
      [
        'link_title' => 'Press',
        'link_path' => 'about-us/press-releases',
      ],
      [
        'link_title' => 'Awards',
        'link_path' => 'about-us/awards',
      ],
      [
        'link_title' => 'Careers',
        'link_path' => 'about-us/careers',
      ],
      [
        'link_title' => 'Travel Butler',
        'link_path' => 'before-you-fly/travel-butler',
      ],
    ],
    FCL_FCUK_FOOTER_2017_MENU_CONTACT => [
      [
        'link_title' => 'Store Finder',
        'link_path' => 'store-locator',
      ],
      [
        'link_title' => 'Contact us',
        'link_path' => 'contact-us',
      ],
      [
        'link_title' => 'Feedback',
        'link_path' => 'contact-us/feedback',
      ],
      [
        'link_title' => 'Customer Services',
        'link_path' => 'customer-services',
      ],
    ],
  ];

  foreach ($menu_links_by_menu as $menu_name => $menu_links) {
    foreach ($menu_links as $weight => $menu_link) {

      $menu_link['weight'] = $weight;
      $menu_link['menu_name'] = $menu_name;

      // If this is a node, we need the node ID.
      if ($path = drupal_lookup_path('source', $menu_link['link_path'])) {
        $menu_link['link_path'] = $path;
        $menu_link['router_path'] = 'node/%';
      }

      if ($plid = menu_link_save($menu_link)) {
        drupal_set_message(t('Created menu link @plid for path @path in menu @menu.', [
          '@plid' => $plid,
          '@path' => $menu_link['link_path'],
          '@menu' => $menu_name,
        ]), 'status');
      }
      else {
        drupal_set_message(t('Couldn\'t create menu link @plid for path @path in menu @menu.', [
          '@plid' => $plid,
          '@path' => $menu_link['link_path'],
          '@menu' => $menu_name,
        ]), 'error');
      }
    }
  }
}
