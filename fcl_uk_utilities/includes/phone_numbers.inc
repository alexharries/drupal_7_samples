<?php
/**
 * @file
 * Phone number-related functions.
 */

/**
 * Strips non-numeric characters (including spaces) from a phone number.
 *
 * Some examples:
 *
 * "+44 7731 530233" would become "447731530233" (technically a valid number).
 * "Call: 020 7123 9999" becomes "02071239999"
 * "Monkeys, direct to your door: 07981 202100" ends up as "07981202100"
 *
 * This function isn't foolproof though:
 *
 * "Call 020 9221 8484 now for red hot toaster-on-toaster action until 2am"
 * becomes "020922184842" (note the 2 on the end).
 *
 * So, you know, this should only be used on phone numbers, ok? :)
 *
 * @param string $number
 *   A phone number string which may or may not include non-numeric characters.
 *
 * @return string
 *   The numeric characters from $number.
 */
function fcl_uk_utilities_clean_phone_number($number) {
  return preg_replace('/\D/', '', $number);
}
