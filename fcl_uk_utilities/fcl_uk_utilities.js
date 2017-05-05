/**
 * This adds some new functions to jQuery.
 *
 * Note that the usual namespacing and Drupal arguments, etc. are not required.
 */
(function ($) {
  'use strict';
  $.fn.extend({
    /**
     * Bind a handler before any already bound handlers for an event.
     *
     * @param string eventType
     * @param function handler
     */
    bindEventHandlerAtStart: function(eventType, handler) {
      // Grab the elements from the current DOM context.
      var $elements = $(this),
         _data;

      $elements.bind(eventType, handler);
      // This binds the event, naturally, at the end of the event chain. We
      // need it at the start.

      if (typeof $._data === 'function') {
        // Since jQuery 1.8.1, it seems, that the events object isn't
        // available through the public API `.data` method.
        // Using `$._data, where it exists, seems to work.
        _data = true;
      }

      $elements.each(function (index, element) {
        var events;

        if (_data) {
          events = $._data(element, 'events')[eventType];
        }
        else {
          events = $(element).data('events')[eventType];
        }

        events.unshift(events.pop());

        if (_data) {
          $._data(element, 'events')[eventType] = events;
        }
        else {
          $(element).data('events')[eventType] = events;
        }
      });
    },
    /**
     * Fetch the GET parameters and return them as an object.
     */
    getUrlParameters: function getUrlParameters() {
      var vars = {};
      var hash;
      var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');
      for (var i = 0; i < hashes.length; i++) {
        hash = hashes[i].split('=');
        vars[hash[0]] = hash[1];
      }
      return vars;
    }
  });
})(jQuery);
