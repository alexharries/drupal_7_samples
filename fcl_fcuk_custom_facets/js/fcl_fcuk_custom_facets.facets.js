(function($, Drupal, window, document, undefined) {
  var FCUK = window.FCUK || {};
  FCUK.CUSTOMFACETS = {
    url: [],
    firstClick: true,
    timeout: null,
    params: {},
    currentURL: window.location.pathname,
    destinationIn: '',
    spinnerOpts: {  // Spinner options
      lines: 13, // The number of lines to draw
      length: 20, // The length of each line
      width: 2, // The line thickness
      radius: 9, // The radius of the inner circle
      corners: 1, // Corner roundness (0..1)
      rotate: 0, // The rotation offset
      direction: 1, // 1: clockwise, -1: counterclockwise
      color: '#000', // #rgb or #rrggbb or array of colors
      speed: 1, // Rounds per second
      trail: 60, // Afterglow percentage
      shadow: false, // Whether to render a shadow
      hwaccel: false, // Whether to use hardware acceleration
      className: 'spinner-holidays-facet', // The CSS class to assign to the spinner
      zIndex: 2e9, // The z-index (defaults to 2000000000)
      top: '50%', // Top position relative to parent
      left: '50%' // Left position relative to parent
    },

    init: function() {
      this.checkboxEventListener();
      //this.updateDestinationLoad();
      this.addFacetIcon();
    },
    addFacetIcon: function() {
      //due to missing icon on Holiday type facet filter
      //add span with toggle class to any h2s with fcl-facets-collapsible
      $('h2.fcl-facets-collapsible:not(:has(span))').append('<span class="toggle"> </span>');
    },
    checkboxGroup: function($this) {
      var parentClass = $this.closest('ul.fcl-facets-fcl-facets-checkboxes').attr("class"),
        group = "",
        classes = {
          'fcl-facets-facet-ss-destination': 'destination',
          'fcl-facets-facet-ss-departure': 'departure',
          'fcl-facets-facet-flight-class': 'flight_class',
          'fcl-facets-facet-ss-product-supplier': 'airlines',
          'fcl-facets-facet-ss-product-duration': 'duration_in',
          'fcl-facets-facet-iss-duration-days': 'duration_days',
          'fcl-facets-facet-ss-star-rating': 'star_rating',
          'fcl-facets-facet-sm-vid-product-experience': 'type',
          'fcl-facets-facet-ss-product-selection': 'holiday_range',
          'fcl-facets-facet-sm-itinerary-destination-full': 'destination',
          'fcl-facets-facet-im-mercury-holiday-segments-destinations-tids': 'destination',
          'fcl-facets-facet-fs-price': 'price',
          'fcl-facets-facet-fs-price-gbp': 'price_gbp',
          'fcl-facets-facet-sm-mercury-holiday-type': 'type',
          'fcl-facets-facet-ss-mercury-duration-days-night': 'duration_in',
          'fcl-facets-facet-is-mercury-holiday-price-from': 'price',
          'fcl-facets-facet-fm-mercury-holiday-hotel-star-rating': 'star_rating'
        };

      $.each(classes, function(index, value) {
        if (parentClass.indexOf(index) > -1) {
          group = value;
        }
      });
      return group;
    },

    // Used to set the destination checkboxes based on the destination_in values on load
    updateDestinationLoad: function() {
      var match = window.location.search.match(/destination_in=([^&]+)/),
        decodedDestinationIn,
        $title,
        $facetMatch,
        $deselectLink;

      if (match !== null && match.length > 1) {
        // Normalise and set destinationIn.
        decodedDestinationIn = decodeURIComponent(match[1]);
        this.destinationIn = decodedDestinationIn.toLowerCase().replace(/\b[a-z]/g, function(letter) {
          return letter.toUpperCase();
        });
        this.destinationIn = this.destinationIn.replace('+', ' ');
        // Set the h2 tag to include the context.
        $title = $('.field-name-field-h1-title h1');
        $title.text($title.text() + ' results for ' + this.destinationIn);
        $facetMatch = $('.fcl-facets-facet-ss-destination, .fcl-facets-facet-sm-itinerary-in').find('input[value="' + this.destinationIn + '"]');
        if ($facetMatch.length === 1) {
          $facetMatch.attr('checked', true);
          // Style the destination text to match the normal facet behaviour.
          $deselectLink = $('<a>' + this.destinationIn + '</a>');
          $facetMatch.siblings('a.fcl-facets-inactive').hide().after($deselectLink);
          $deselectLink.click(function() {
            var $checkbox = $(this).siblings('.fcl-facets-checkbox');
            $checkbox.attr('checked', !$checkbox.attr('checked')).trigger('change');
          });
          this.updateParams.call($facetMatch);
        }
      }
    },

    checkboxEventListener: function() {
      $('.fcl-facets-checkbox').unbind();
      var _this = this,
        groupName = "",
        destFacetClass = "fcl-facets-facet-im-mercury-holiday-segments-destinations-tids";

      $('.fcl-facets-checkbox').each(function(index) {
        var oldFilterText,
          cache;

        if ($(this).attr('checked')) {
          // Check which group the checkbox belongs to.
          groupName = _this.checkboxGroup($(this));
          checkedValue = _this.fclEncodeURIComponent($(this).val());
          // Set the group array.
          if (_this.params[groupName] === undefined) {
            _this.params[groupName] = {};
          }
          _this.params[groupName][checkedValue] = checkedValue;
          // Change the labels for selected filters into clickable links.
          $parent = $(this).parent();
          oldFilterText = $parent.clone().children().remove().end().text();
          var checkbox = $parent.children('.fcl-facets-checkbox');
          var nested = $parent.children().not('.fcl-facets-checkbox');
          $parent.html('<a href="">' + oldFilterText + '</a>').prepend(checkbox).append(nested);
        }
      });

      // Clicking on the links also changes checkbox values.
      $('.fcl-facets-checkbox').siblings('a').click(function(ev) {
        
        groupName = _this.checkboxGroup($(this));
        if (groupName === "destination") {
          spinner = new Spinner(_this.spinnerOpts).spin();
          $('body').append(spinner.el);
          $('body').append('<div class="overlay"></div>');
        }
        var $checkbox = $(this).siblings('.fcl-facets-checkbox');

        ev.preventDefault();
        $checkbox.attr('checked', !$checkbox.attr('checked')).trigger('change');
      });

      // Load page on checkbox change (not click, as links need to trigger).
      $('.fcl-facets-checkbox').change(function(ev) {
        ev.stopPropagation();
        clearTimeout(_this.timeout);

        // Add spinner to destination facet.
        groupName = _this.checkboxGroup($(this));
        if (groupName === "destination") {
          spinner = new Spinner(_this.spinnerOpts).spin();
          $('body').append(spinner.el);
          $('body').append('<div class="overlay"></div>');
        }
        
        _this.updateParams.call(this);

        // Uncheck children in hierarchical facet.
        var $children = $(this).not(":checked").parent().find('.fcl-facets-checkbox:checked');
        $children.attr("checked", false).each(function() {
          _this.updateParams.call(this);
        });

        _this.timeout = setTimeout(function() {
          _this.generateUrl();
        }, 2000);

      });
    },

    updateParams: function() {
      var _this = FCUK.CUSTOMFACETS,
        checkedValue,
        groupName,
        encodedDestination;

      checkedValue = _this.fclEncodeURIComponent($(this).val());
      // Check which group the checkbox belongs to.
      groupName = _this.checkboxGroup($(this));

      // Set the group array.
      if (_this.params[groupName] === undefined) {
        _this.params[groupName] = {};
      }
      // If is checked add the value to the group.
      if ($(this).attr('checked')) {
        _this.params[groupName][checkedValue] = checkedValue;
      }
      else {
        delete _this.params[groupName][checkedValue];
        // If deselecting the destination_in from Destination facet, remove it.
        encodedDestination = _this.fclEncodeURIComponent(_this.destinationIn);
        if (groupName === 'destination' && checkedValue === encodedDestination) {
          _this.destinationIn = '';
        }
      }
      // Remove group if empty.
      if ($.isEmptyObject(_this.params[groupName])) {
        delete _this.params[groupName];
      }
    },

    // Special characters get indexed as entities, so we need to convert them
    // to entities before encoding the string.
    fclEncodeURIComponent: function(n) {
      var map = {
        '&': '&amp;',
        '<': '&lt;',
        '>': '&gt;',
        '"': '&quot;',
        "'": '&#039;',
        // Double url-encoding because commas get indexed as commas.
        // @TODO: This is disabled for now until we can handle the "London"
        // facet properly. See: fcl_fcuk_forms_alterations_submit_alter().
        /*
         ",": '%2C'
         */
      };
      n = n.replace(/[&<>"']/g, function(m) {
        return map[m];
      });
      return encodeURIComponent(n);
    },

    // Generate the URL.
    generateUrl: function() {
      var i = 0,
        j = 0,
        _this = this,
        locationURL = _this.currentURL,
        destinationInQuery = '',
        prefix;

      $.each(this.params, function(groupName, paramValues) {
          firstparm = false;
          if (i === 0) {
            locationURL += '?' + groupName + '=';
            firstparm = true;
          }
          else {
            locationURL += '&' + groupName + '=';
            firstparm = true;
          }
          $.each(paramValues, function(paramValue, value) {

            if (firstparm) {
              locationURL += value;
              firstparm = false;
            }
            else {
              locationURL += ',' + value;
            }
            j++;
          });
          i++;
        }
      );

      // Append the destination_in query string to the URL if present.
      if (_this.destinationIn !== '') {
        prefix = '&';
        if (locationURL === _this.currentURL) {
          prefix = '?';
        }
        destinationInQuery = prefix + 'destination_in=' + _this.destinationIn;
      }
      locationURL += destinationInQuery;
      // Load the URL in the browser.
      window.location = locationURL;
    }
  }
  $(function() {
    FCUK.CUSTOMFACETS.init();
  });
})
(jQuery, Drupal, this, this.document);
