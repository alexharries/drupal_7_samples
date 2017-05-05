<?php
/**
 * @file
 * Email-related functions.
 */

/**
 * Basic checking on whether an email address is valid.
 *
 * @param string $email
 *   Raw value.
 *
 * @return bool
 *   Whether or not the address is valid
 */
function fcl_uk_utilities_is_valid_email_address($email) {
  // Start with Drupal's default validator.
  if (!valid_email_address($email)) {
    return FALSE;
  }
  
  // Ok, that worked. Now let's do some custom checks, too.
  $email = check_plain($email);
  
  // If it's number@flightcentre.co.uk then it's generated.
  $email_parts = explode('@', $email);
  $prefix = $email_parts[0];
  $suffix = $email_parts[1];
  if (is_numeric($prefix) && $suffix == 'flightcentre.co.uk') {
    return FALSE;
  }
  
  return TRUE;
}
