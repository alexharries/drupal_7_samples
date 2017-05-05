<?php

/**
 * @file
 * Constant definitions for Flight Centre Communication Pods.
 */

/**
 * The filenames for communication pods.
 */
const COMMUNICATION_POD_TEMPLATE_EXTENSION = '.tpl.html';
const COMMUNICATION_POD_INFO_EXTENSION = '.info';
const COMMUNICATION_POD_CSS_EXTENSION = '.css';
const COMMUNICATION_POD_JS_EXTENSION = '.js';

/**
 * The suffix to append to purposefully-disabled communication pods.
 */
const COMMUNICATION_POD_POD_DISABLED = '__disabled';

/**
 * The theme function prefix for communication pods.
 */
const COMMUNICATION_POD_THEME_FUNCTION = 'communication_pod';

/**
 * The cache lifetime, in seconds.
 */
const COMMUNICATION_POD_CACHE_LIFETIME = (15 * 60);

/**
 * The following constants define the names of fields/database schema keys.
 *
 * This is the schema's name.
 */
const COMMUNICATION_PODS_SCHEMA_NAME = 'fcl_fcuk_communication_pods';

/**
 * The name of the pod's machine name column.
 */
const COMMUNICATION_POD_MACHINE_NAME = 'pod_machine_name';

/**
 * The name of the human-readable pod name column.
 */
const COMMUNICATION_POD_FRIENDLY_NAME = 'pod_name';

/**
 * Name of the entity ID column.
 */
const COMMUNICATION_POD_ID_FIELD = 'id';

/**
 * The name of the language column.
 */
const COMMUNICATION_POD_LANGUAGE_FIELD = 'language';

/**
 * The name of the Communication Pod entity.
 */
const COMMUNICATION_PODS_ENTITY_NAME = 'communication_pod';

/**
 * The name of the Communication Pod bundle.
 */
const COMMUNICATION_PODS_BUNDLE_NAME = COMMUNICATION_PODS_ENTITY_NAME;

/**
 * The URI callback for the entity controller.
 */
const COMMUNICATION_PODS_ENTITY_URI_CALLBACK = 'fcl_fcuk_communication_pods_pod_uri';

/**
 * The label callback for the entity controller.
 */
const COMMUNICATION_PODS_ENTITY_LABEL_CALLBACK = 'fcl_fcuk_communication_pods_pod_label';

/**
 * And the Communication Pod entity controller.
 */
const COMMUNICATION_PODS_ENTITY_CONTROLLER = 'CommunicationPodController';

/**
 * Flags used to indicate if a pod was found or not.
 */
const COMMUNICATION_POD_FOUND = 'COMMUNICATION_POD_FOUND';
const COMMUNICATION_POD_NOT_FOUND = 'COMMUNICATION_POD_NOT_FOUND';

/**
 * The base path for the Communication Pods administration section.
 */
const COMMUNICATION_PODS_ADMIN_URL = 'admin/content/communication-pods';

/**
 * The base path for a Communication Pod's view URL.
 */
const COMMUNICATION_POD_ENTITY_PATH_SLUG = 'communication-pods';

/**
 * Communication pod statuses (statii?).
 *
 * Communication pods which are loaded direct from disk and have no settings
 * in the database.
 */
const COMMUNICATION_POD_STATUS_CODE_DEFAULT = 'in code - default';

/**
 * Communication pods which originated from disk and have been overridden
 * in the database, for example to change its settings.
 */
const COMMUNICATION_POD_STATUS_CODE_OVERRIDDEN = 'in code - overridden';

/**
 * Communication pods which were created in the administration interface only,
 * and whose machine name doesn't match any pod on disk.
 */
const COMMUNICATION_POD_STATUS_DB = 'custom';

/**
 * Permissions.
 */
const COMMUNICATION_POD_PERM_ADMIN_OVERVIEW = 'access communication pods admin overview';
const COMMUNICATION_POD_PERM_CREATE = 'create communication pods';
const COMMUNICATION_POD_PERM_EDIT_OWN = 'edit own communication pods';
const COMMUNICATION_POD_PERM_EDIT_ANY = 'edit any communication pod';
const COMMUNICATION_POD_PERM_DELETE_OWN = 'delete own communication pods';
const COMMUNICATION_POD_PERM_DELETE_ANY = 'delete any communication pod';
const COMMUNICATION_POD_PERM_EDIT_JS = 'edit custom javascript';

/**
 * Permission grants.
 */
const COMMUNICATION_POD_ACCESS_GRANTED = TRUE;
const COMMUNICATION_POD_ACCESS_DENIED = FALSE;

/**
 * Actions.
 */
const COMMUNICATION_POD_ACTION_CREATE = 'create';
const COMMUNICATION_POD_ACTION_READ = 'read';
const COMMUNICATION_POD_ACTION_UPDATE = 'edit';
const COMMUNICATION_POD_ACTION_OVERRIDE = 'override';
const COMMUNICATION_POD_ACTION_DELETE = 'delete';
const COMMUNICATION_POD_ACTION_REVERT = 'revert';
