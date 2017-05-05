/**
 * @file
 */
(function($) {
  Drupal.behaviors.ajaxSearchFormsSearch = {
    attach: function(context, settings) {
      this.init(context, settings);
    },

    /**
     * Set everything up.
     */
    init: function(context, settings) {
      this.initialiseSelect2();

      // Set up an onChange listener on the selects.
      var $selects = $('select[data-ajax-search-forms-enabled="1"]');

      if ($selects.size() > 0) {
        // There are selects in the dom that need listeners.
        $selects.on('change', function(e) {

          // Get some elements.
          var $form = $(this).closest('form');

          // Reset all fields to the right of/subsequent to the changed field.
          var $selectsToTheRightOfThisSelect = $(this).closest('.form-item').nextAll('.form-item').find('select');
          $selectsToTheRightOfThisSelect.attr('value', null).trigger('change');

          // Check if there are any subsequent selects after this one which
          // will need to be updated via Ajax when this one is changed.
          if ($selectsToTheRightOfThisSelect.length > 0) {
            var $subsequentSelects = $(this).closest('.form-item').nextAll('.form-item').find('select[data-ajax-search-forms-enabled="1"]');

            $subsequentSelects.each(function(i) {
              // Get the URL query name of the select, e.g. destinations or
              // star_rating, etc.
              this.updateSelectOptions($(this));
            });
          }
        });
      }
    },

    /**
     * Update the select options in a drop-down select via Ajax.
     *
     * @param $dropDownSelect
     *   The jQuery drop-down select element.
     */
    updateSelectOptions: function($dropDownSelect) {
      // Don't process this select if an Ajax request is already running on it.
      if ($dropDownSelect.data('ajax-search-forms-updating') == 1) {
        return;
      }

      // Set a flag to indicate we're updating the element.
      $dropDownSelect.data('ajax-search-forms-updating', 1);

      // Do we want to call the cached or uncached Ajax callback?
      // (We assume it should be cached if not specifically uncached).
      if ($dropDownSelect.data('cache-enabled') === 1) {
        var cacheOrNoCacheURLSlug = 'cache';
      }
      else {
        var cacheOrNoCacheURLSlug = 'nocache';
      }

      // Get the selectUrlQueryName value.
      var selectUrlQueryName = $dropDownSelect.attr('name'),

        // Get the form's context and callback path.
        context = $dropDownSelect.data('context'),
        dataCallbackPath = $dropDownSelect.data('callback-path'),

        // Assemble a URL based on the current form values.
        url = Drupal.settings.basePath + 'ajax/' + cacheOrNoCacheURLSlug + '/' + dataCallbackPath + '/' + context + '/' + selectUrlQueryName + '?',

        // Get some elements.
        $form = $dropDownSelect.closest('form');

      // Append any key and value pairs to the Ajax URL.
      $form.find('select').each(function() {
        var key = $(this).prop('name'),
          val = $(this).val();
        if (val !== '') {
          url += encodeURIComponent(key) + '=' + encodeURIComponent(val) + '&';
        }
      });

      // Add the element's ID into the request so that the response can contain
      // the right selector to target this element.
      url += 'id=' + $dropDownSelect.attr('id') + '&';

      // Set the drop-down's option to be "Updating..." with a null value so
      // that any subsequent Ajax requests for fields to the right of this one
      // don't incorrectly try to filter on this field's value.
      $dropDownSelect.html('<option value="" disabled selected="selected">' + Drupal.t('Updating...') + '</option>');

      // Send an ajax request to the url.
      $.get(url, null, holidaySearchUpdateOptions);
    },

    /**
     * This function will find all elements on the page with a data-use-select2
     * attribute set to 1 and will initialise Select2 on them.
     */
    initialiseSelect2: function() {
      $('body').find('[data-use-select2="1"]').each(function() {
        var id = '#' + $(this).attr('id'),
          opts = new Drupal.behaviors.fcukStaticFormsSelect2.getOpts(id);

        Drupal.behaviors.fcukStaticFormsSelect2.configureSelect2(id, 2, opts.tooShort, opts.typeText, null);
      });
    },
  };

  /**
   * Update a select's options with new data from the server. Yay, Ajax! :D
   *
   * @param object response
   *   A response object which should contain a jQuery {selector} string so we
   *   can identify the select element to be updated, and a chunk of HTML
   *   containing <option /> elements.
   */
  var holidaySearchUpdateOptions = function(response) {
    if (typeof response !== 'undefined') {
      $(response.data.selector).html(response.data.options).data('ajax-search-forms-updating', 0);
    }
  };
})(jQuery);
