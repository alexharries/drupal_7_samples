/**
 * @file
 * Run Validation on forms with a GIMP form base.
 *
 * Each form can provide an array of settings to define its own custom validation handler
 * and to instantiate it's own validation rules.
 *
 */
(function($, Drupal, window, document, undefined) {
  'use strict';
  // Validation handler that works for the
  Drupal.behaviors.fclukUtilitiesFormValidation = {
    validationHandlerStoreAppointmentModal: function(formid, currentStaticForm, validationRules, settings) {
      // @TODO The following code enables us to provide custom routines when there is an invalid form element.
      // ideally we should be able to override the InvalidHandler function but it's not working as expected,
      // something to do with this being a modal form, so fo the time being we are binding to the high level
      // events as triggered by fsf.form_validate.js.
      $('form').on("validate:form:invalid", function(event, validator) {
        if (validator.currentTarget.id = formid) {
          $('#' + formid + ' ' + '.form-validation').show();
        }
      });

      $('form').on("validate:element:ok", function(event, element) {
        if (event.currentTarget.id = formid) {
          if (!$('#' + formid + ' label').hasClass("error")) {
            $('#' + formid + ' ' + '.form-validation').hide();
          }
        }
      });

      Drupal.behaviors.fclukUtilitiesFormValidation.validationAddRulesDefault(validationRules);
    },

    // Default validation handler
    validationHandlerDefault: function(formid, currentStaticForm, validationRules, settings) {
      Drupal.behaviors.fclukUtilitiesFormValidation.validationAddRulesDefault(validationRules);
    },

    // Default validation handler
    validationAddRulesDefault: function(validationRules) {
      // Loop through the validation rules as required for this form to add them.
      $.each(validationRules, function(validationMethodName, validationRule) {
        try {
          eval('Drupal.behaviors.'
            + validationRule.behavior
            + '.' + validationMethodName + '()');
        }
        catch (e) {
          console.log(e.message);
        }
      })
    },

    attach: function(context, settings) {
      var forms = {},
        validationSettings = settings.fclukStaticFormValidationOverrides.validationSettings,
        formId,
        validationHandlerMethod = '',
        validationBehavior = '',
        validationRules = new Array();
      // Loop through the forms that require validation rules applied to them.
      $.each(validationSettings.forms, function(formIdentifier, form) {
        // Some forms dynamically generate the form id in which case we'll get the id by class name.
        if  (form.formIdentifierType === 'class') {
          formId = $('form.' + formIdentifier).attr('id');
        }
        else {
          formId = validationSettings.forms.formIdentifier;
        }
        var $staticForm = $('#' + formId, context);

        // Ensure we process the form only once.
        if ($(form).hasClass('validation-processed') || formId === 'undefined') {
          return;
        }
        // Set variables where we there are settings defined.
        if (form.validationHandler.method !== undefined) {
          validationHandlerMethod = form.validationHandler.method;
        }
        if (form.validationHandler.behavior !== undefined) {
          validationBehavior = form.validationHandler.behavior;
        }
        if (form.validationRules !== undefined) {
          validationRules = form.validationRules;
        }

        // Define the function to call for our custom validation handler.
        var callValidationHandlerFunc = 'Drupal.behaviors.'
          + validationBehavior
          + '.' + validationHandlerMethod;
        try {
          // Call our custom validation handler.
          eval(callValidationHandlerFunc
            + '(formId, $staticForm, validationRules, settings);');
        }
        catch (e) {
          // Call our custom default validation handler.
          eval('Drupal.behaviors.fclukUtilitiesFormValidation'
            + '.validationHandlerDefault'
            + '(formId, $staticForm, validationRules, settings)');
          console.log(e.message + '. Calling default validation handler instead.');
        }
      });
    }
  };
})(jQuery, Drupal, this, this.document);
