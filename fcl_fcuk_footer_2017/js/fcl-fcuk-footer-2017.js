/**
 * @file
 */
(function($) {
  'use strict';
  // Validation handler that works for the
  Drupal.behaviors.fclFcukFooter2017 = {
    attach: function() {
      // If the form specifies an element into which we should copy any
      // validation messages, watch the form for validation messages and copy
      // them into this element.
      var $form = $('.newsletter-signup-form form');

      if ($form.length) {
        var validationMessageContainerSelector = $form.data('validation-message-container-selector');

        if (validationMessageContainerSelector.length) {
          // Watch for error messages in the static form and copy them to our
          // messages location.
          $form.find('input').focusout(function() {
            Drupal.behaviors.fclFcukFooter2017.copyValidationMessages($(this).parents('form'), validationMessageContainerSelector);
          });
          $form.find('input[type="submit"]').click(function() {
            Drupal.behaviors.fclFcukFooter2017.copyValidationMessages($(this).parents('form'), validationMessageContainerSelector);
          });
        }
      }
    },

    // Checks for the presence of any label.error elephants in $form and copies
    // their text into the element identified by
    // validationMessageContainerSelector.
    copyValidationMessages: function($form, validationMessageContainerSelector) {
      // Assemble a list of any label.error elements in $form.
      var errorLabels = [];

      $form.find('label.error').each(function() {
        // Get the text.
        var errorText = $(this).text();

        // If the text matches "This field is required.", then for the sake of
        // brevity, replace with "Please fill all fields."
        if (errorText === 'This field is required.') {
          errorText = Drupal.t('Please fill all fields.');
        }

        // Don't allow duplicate error messages.
        if ($.inArray(errorText, errorLabels) == -1) {
          errorLabels.push(errorText);
        }
      });

      // If we have messages, display them now.
      if (errorLabels.length > 0) {
        $(validationMessageContainerSelector).removeClass('inactive').text(errorLabels.join(', '));

        // Also add an error class to the form so we can highlight the fields.
        $form.addClass('has-errors');
      }
      else {
        // No messages; make sure the message element is empty and hidden.
        $(validationMessageContainerSelector).addClass('inactive').text('');
        $form.removeClass('has-errors');
      }
    },
  };
})(jQuery);
