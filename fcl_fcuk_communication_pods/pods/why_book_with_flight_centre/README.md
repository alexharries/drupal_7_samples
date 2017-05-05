# What's this?

This directory contains a template for a communication pod. It can contain the following files:

- pod.php: a Drupal PHP template file containing the communication pod's HTML and any server-side PHP code. This file will be run from within Drupal and should conform to Drupal coding standards, for example making correct use of the database abstraction layer,
- pod.css: any CSS styles for your pod. This file will be automagically loaded if present. It's up to you to make sure you create styles which correctly target your communication pod, and only your pod, so you should wrap your pod's HTML in a unique class (not an ID - a pod may be repeated on the same page in some circumstances),
- pod.js: any Javascript for your pod. You should write your code in-line with the [JS guidelines](https://confluence.au.flitech.net/display/FCUK/JS+Guidelines).

If you want to add images, place them in the ./images directory and add them in to your pod like this:

`<img src="{pod_url}/images/monkey.jpg" alt="A monkey" title="A picture of a monkey hanging onto a drainpipe." />`
