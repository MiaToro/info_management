<?php

/**
  * @file
  * functions for info_management module
  */ 
  
/**
  * Generate the info_management add form array 
  * 
  * @see info_management_display_data()
  */
function info_management_input_data($form, &$form_state) {
  
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Data Input'),
	'#title_display' => 'invisible',
  );
  
  // This is the form elements.
  $form['name'] = array(
    '#type' => 'fieldset',
    //'#title' => t('Personal Information'),
	 // Make the fieldset collapsible.
    //'#collapsible' => TRUE,
    //'#collapsed' => FALSE,
  ); 
  $form['name']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name in English:'),
	'#size' => 60,
    '#maxlength' => 60,
  );
  $form['name']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name in English:'),
	'#size' => 60,
    '#maxlength' => 60,
  );
  $form['name']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email:'),
	'#size' => 60,
    '#maxlength' => 60,
  );  

  $form['name']['col_industry'] = array(
    '#type' => 'select',
    '#title' => t('Industry:'),
	'#options' => info_management_subtable('info_management_industry', 'i_id'),
  );  
  $form['actions']['reset'] = array( 
    '#markup' => '<input class="info-management-reset-submit form-submit" type="reset" value="' . t("Reset"). '"/>',  
  );  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
	'#submit' => array('info_management_input_submit'),
	'#attributes' => array(
	   'class' => array('info-management-input-submit'),
	),	
  );  

  return $form;
}
 
/**
  * Submit handler for input_data().
  */
function info_management_input_submit($form, &$form_state) {
  try {
	$id = db_insert('info_management')
	->fields(array(
	  'first_name' => check_plain($form_state['values']['first_name']),
	  'last_name' => check_plain($form_state['values']['last_name']),
	  'email' => check_plain($form_state['values']['email']),
	  'col_industry' => check_plain($form_state['values']['col_industry']),
	))
	->execute();
	
	drupal_set_message(t('The form has been submitted. name="@first_name @last_name", Email="@email"',
	  array(
		'@first_name' => $form_state['values']['first_name'],
		'@last_name' => $form_state['values']['last_name'],
		'@email' => $form_state['values']['email'],
	  )
	));  
	return $id;
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	$txn->rollback();
	// Log the exception to watchdog.
	watchdog_exception('type', $e);
  }
}

/**
  * Menu callback: content management.
  */
function info_management_display_data($form, $form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    drupal_set_message(count($form_state['values']['records']));
    return info_management_multiple_delete_confirm($form, $form_state, array_filter($form_state['values']['records']));
  }
  
  $form['filter'] = info_management_filter_form();
  $form['#submit'][] = 'info_management_filter_submit'; 
  $form['management'] = info_management_data();  
  return $form;
}

/**
 * Return form for record adminstration filters.
 */
function info_management_filter_form() {
  $session = isset($_SESSION['record_overview_filter']) ? $_SESSION['record_overview_filter'] : array();
  
  // This is the form elements.
  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
  //'#attributes' => array('class' => array('container-inline')),  
	//'#theme' => 'exposed_filters__record',
	 /* Make the fieldset collapsible.
    '#collapsible' => TRUE,
    '#collapsed' => FALSE, */
  ); 
  //    '#markup' => t('SHOW ONLY ITEMS WHERE'),
  
  $i = 0;
  foreach ($session as $key => $filter) {
    $t_args =array('%property' => $key, '%value' => $filter);
	if ($i++) {
	  $form['filter']['current'][] = array('#markup' => t('<br />and %property includes %value', $t_args));  
	}
	else {
	  $form['filter']['current'][] = array('#markup' => t('where %property includes %value', $t_args));  
	}  
  }
  $form['filter']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name in English:'),
	'#size' => 60,
    '#maxlength' => 60,
  );
  $form['filter']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name in English:'),
	'#size' => 60,
    '#maxlength' => 60,
  );
  $form['filter']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email:'),
	'#size' => 60,
    '#maxlength' => 60,
  );
  
  $form['filter']['col_industry'] = array(
    '#type' => 'select',
    '#title' => t('Industry:'),
	'#options' => info_management_subtable('info_management_industry', 'i_id'),
	'#default_value' => 1,
  );
  
  $form['filter']['actions']['submit'] = array(
	'#type' => 'submit',
	'#value' => (count($session)) ? t('Refine') : t('Filter'),
	'#attributes' => array(
	  'class' => array('info-management-submit'),
	),
  );
  if(count($session)) {
	$form['filter']['actions']['undo'] = array(
	  '#type' => 'submit',
	  '#value' => t('Undo'),
	  '#attributes' => array(
		'class' => array('info-management-undo'),
	  ),
	);
	$form['filter']['actions']['reset'] = array(
	  '#type' => 'submit',
	  '#value' => t('Reset'),
	  '#attributes' => array(
		 'class' => array('info-management-reset'),
	  ),
	);
  }
  
//  drupal_add_js(misc/form.js);
  return $form;
} 

/**
 * Process result from record administration filter form.
 */
function info_management_filter_submit($form, &$form_state) {  
  switch ($form_state['values']['op']) { 
    case t('Filter'):
	case t('Refine'):
	  $filters = info_management_filters();
	  $filters_key = array_keys($filters);
	  foreach ($filters_key as $key) {
			// drupal_set_message($key . '!=>' . $form_state['values'][$key]);
		  if (trim($form_state['values'][$key]) != null) {
			$_SESSION['record_overview_filter'][$key] = $form_state['values'][$key];
			// drupal_set_message($key . '=>' . $form_state['values'][$key]);  
		  }  
	  }
	  drupal_set_message(t('Filter/Refine is submitted.')); 
	  break;
	case t('Undo'):
      array_pop($_SESSION['record_overview_filter']);
      drupal_set_message(t('Undo is submitted.'));
	  break;
	case t('Reset'):
      $_SESSION['record_overview_filter'] = array();
      drupal_set_message(t('Reset is submitted.'));
	  break;  
	}
}    
 
/**
  * Form builder: Builds the data management overview.
  */  
function info_management_data() {  
  $user_access =  1; //user_access('Access content for the Information Management module');
  
  // Build the 'Update options' form.
  $form['options'] = array(
	'#type' => 'fieldset',
	'#title' => t('Update options'),
	'#attributes' => array('class' => array('container-inline')),
	'#access' => $user_access,
  );
  
  $options = array();
  foreach (info_management_operations() as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(
	'#type' => 'select',
	'#title' => t('Operation'),
	'#title_display' => 'invisible',
	'#options' => $options,
	'#default_value' => '',
  );
  $form['options']['submit'] = array(
	'#type' => 'submit',
	'#value' => t('Update'),
	'#attributes' => array('class' => array('info-management-update')),
 	'#validate' => array('info_management_records_validate'),
 	'#submit' => array('info_management_records_submit'),
  );
	
  // Build the sortable table header  
  $header = array(
    'id' => array('data' => t('ID'), 'field' => 'n.id', 'sort' => 'desc'),  
	'first_name' => array('data' => 'First Name', 'field' => 'n.first_name'),
	'last_name' => array('data' => 'Last Name', 'field' => 'n.last_name'),
	'email' => array('data' => t('Email'), 'field' => 'n.email'), 
	'col_industry' => array('data' => t('Industry'), 'field' => 'n.col_industry'), 
	'operation' =>  t('Operation'), 
  );
  
  // Use Database API to retrieve records.
  $max_num = variable_get('info_management_max', 3);
  $query = db_select('info_management', 'n')->extend('PagerDefault')->extend('TableSort');
  _record_build_filter_query($query);
  $query->leftJoin('info_management_industry', 'indu', 'n.col_industry = indu.i_id');

  $results = $query
     ->fields('n', array('id', 'first_name', 'last_name', 'email'))
	 ->fields('indu', array('i_id', 'i_c_name', 'i_e_name'))
     ->limit($max_num)
	 ->orderByHeader($header)
     ->addTag('info_management access')
     ->execute();
 
  // Prepare the list of records  
  // Iterate over the resultset and format them.
  $rows = array();
  foreach ($results as $row){
//    $rows[$row->id] = array('id' => $row->id);
    $rows[$row->id] = array('id' => array('data' => $row->id, 'class' => array('info-management-column-id')));
	$rows[$row->id]['first_name'] = array('data' => $row->first_name, 'class' => array('info-management-column-first-name'));
	$rows[$row->id]['last_name'] = array('data' => $row->last_name, 'class' => array('info-management-column-last-name'));
	$rows[$row->id]['email'] = array('data' => $row->email, 'class' => array('info-management-column-email')); 
	$rows[$row->id]['col_industry'] = array('data' => $row->i_e_name . '/' . $row->i_c_name, 'class' => array('info-management-column-industry')); 
	
	// Build a list for all the accessible options for the current node
	$operations = array();
  	if ($user_access) {
	  $operations['edit'] = array(
		'title' => t('Edit'),
		'href' => 'info_management/data/process/3/' . $row->id,
//		'query' => $row->id,
	  );
  	}
    if ($user_access) {
	  $operations['delete'] = array(
		'title' => t('Delete'),
		'href' => 'info_management/data/process/1/' . $row->id,
 //		'query' => $row->id,
	  );
  	}
	$rows[$row->id]['operation'] = array();
	if(count($operations) > 1) {
	  // Render an unordered list of operations links
	  $rows[$row->id]['operation'] = array(
		'data' => array(
		  '#theme' => 'links',
		  '#links' => $operations,
		  '#attributes' => array('class' => array('links', 'inline')),
		),
		'class' => array('info-management-column-operation'),
	  );  
	}
	elseif(!empty($operations)) {
	  // Render the first and only operation as a link
	  $link = reset($operations);
	  $rows[$row->id]['operation'] = array(
		'data' => array(
		  '#type' => 'link',
		  '#title' => $link['title'],
		  '#href' => $link['href'],
		  '#options' => array('query' => $link['query']),
		),
		'class' => array('info-management-column-operation'),
	  );
	} 
  }
 
  // Create a render array ($build) which will be themed as a table with a pager.
  $form['records'] = array(
	'#type' => 'tableselect',
	'#header' => $header,
	'#options' => $rows,
	'#empty' => t('There are no data formats found in the db.'),
 //	'#attributes' => array('class' => array('info-management-table')),
  );
  // Attach the pager theme.
  $form['pager'] = array('#markup' => theme('pager'));   
  return $form; 
}

/**
 * List data administration filters that can be applied.
 */
function info_management_filters() {

  // Regular filters.
  $filters['id'] = array(
	  '#type' => 'textfield',
	  '#title' => t('ID'),
  );  
  $filters['first_name'] = array(
	'#type' => 'textfield',
	'#title' => t('First Name'),
	'#size' => 50,
	'#maxlength' => 50,
  );
  $filters['last_name'] = array(
	'#type' => 'textfield',
	'#title' => t('Last Name'),
	'#size' => 50,
	'#maxlength' => 50,
  );
  $filters['email'] = array(
	'#type' => 'textfield',
	'#title' => t('Email'),
	'#size' => 100,
	'#maxlength' => 100,
  );  
  $filters['col_industry'] = array(
	'#type' => 'textfield',
	'#title' => t('Industry'),
  );      
  
  return $filters;    
}

/**
 * Apply filters for node administration filters based on session.
 *
 * @param $query
 *   A SelectQuery to which the filters should be applied.
 */
function _record_build_filter_query(SelectQueryInterface $query) {
    // Build query
  $filter_data = isset($_SESSION['record_overview_filter']) ? $_SESSION['record_overview_filter'] : array();
  foreach ($filter_data as $filter_key => $filter_value) {
    if (trim($filter_value) != null) {
      $query->condition($filter_key, '%' . db_like($filter_value) . '%', 'LIKE');
  	} 
  }
}


/**
 * Validate info_management_records form submissions.
 * 
 * Check if any records have been selected to perform the chosen
 * 'Update option' on.
 */
function info_management_records_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['records']) || !count(array_filter($form_state['values']['records']))) {
    form_set_error('', t('No items selected'));
  }
}

/**
 * Process info_management_data form submissions.
 *
 * Execute the chosen 'Update option' on the selected nodes.
 */
function info_management_records_submit($form, &$form_state) {
  $operations = info_management_operations();
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked records
  $records = array_filter($form_state['values']['records']);
  
  if ($function = $operation['callback']) {
    // Add in callback arguments if present
/*  if (isset($operation['callback arguments'])) {
	  $args = array_merge(array($records), $operation['callback arguments']);      
    }
    else {
      $args = array($records);
    }   */
  //  call_user_func_array($function, array($form, $form_state, $args));
	if ($function == 'info_management_mass_update') {
	  $_SESSION['record_mass_update'] = $records;
	  // call_user_func_array($function, array($form, $form_state, $args));
	  $form_state['redirect'] = 'info_management/data/process/3/ids';
	}
	// cache_clear_all();
  }
  else {   
    // We need to rebuild the form to go to a second step. For example, to
    // show the confirmation form for the deletion of nodes.
    $form_state['rebuild'] = TRUE;
  }
//  drupal_set_message(t('update is submitted.'));
} 

function info_management_operations() {
  $operations = array(
    'update' => array(
      'label' => t('Mass Update'),
      'callback' => 'info_management_mass_update',
      'callback arguments' => array('arg0' => 'empty'),
    ),
    'delete' => array(
      'label' => t('Delete selected content'),
      'callback' => NULL,
    ),
  );
  return $operations;
}

/**
 * Form to confirm deletion of one record.
 */
function info_management_delete_one_confirm($form, $form_state, $arg0) {
  $form = array();  
  $form['record_no'] = array(
    '#type' => 'value',
    '#value' => $form_state['values']['id'],
  );  
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'info_management_delete_one_submit';
  
  $question = t('<span style="color:#F00;">Delete this No. @record_no record?</span>', array('@record_no' => $form_state['values']['id']));
  $description = t('This will immediately delete the No. @record_no record. This cannot be undone.', array('@record_no' => $form_state['values']['id']));
  return confirm_form($form, 
                      $question, 
					  'info_management/data/process/3' . $form_state['values']['id'], 
					  $description, 
					  t('Delete'), t('Cancel'));
}

/**
  * Submit handler for info_management_delete_data_submit().
  */
function info_management_delete_one_submit($form, &$form_state) {
  $id = $form_state['values']['record_no'];
  if ($form_state['values']['confirm']) {
    _record_delete_multiple(array($id));  
	watchdog('info_management', 'Deleted @id record.', array('@id' => $id));
  }
  $form_state['rebuild'] = TRUE;
  $form_state['redirect'] = 'info_management/data/process/3';
} 

/** 
 * Menu callback -- ask for confirmation of record deletion
 */
function info_management_delete_one_link_confirm($form, &$form_state, $arg0) {
 
  $form = array();
  $form['id'] = array('#type' => 'value', '#value' => $arg0);
  
  return confirm_form($form,
	t('Are you sure you want to delete No. %id record?', array('%id' => $arg0)),
	'info_management/data/process/1/',
	t('This action cannot be undone.'),
	t('Delete'), t('Cancel')
  );
}

function info_management_delete_one_link_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    _record_delete_multiple(array($form_state['values']['id']));
	watchdog('info_management', 'Deleted @id record.', array('@id' => $form_state['values']['id']));
	drupal_set_message('%id has been deleted.', array('%id' => $form_state['values']['id']));
  }
  $form_state['redirect'] = 'info_management/data/process/1/';
}

function info_management_multiple_delete_confirm($form, $form_state, $records) {  
  $form['records'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  // array_filter returns only elements with TRUE values
  foreach ($records as $id => $value) {
  	$first_name = db_query('SELECT first_name FROM {info_management} WHERE id = :id', array(':id' => $id))->fetchField();
	drupal_set_message($id . "=>" . $first_name);
 	$form['records'][$id] = array(
	  '#type' => 'hidden',
	  '#value' => $id,
 	  '#prefix' => '<li>',
 	  '#suffix' => check_plain($first_name) . "</li>\n",
	);   
  }      
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'info_management_delete_confirm_submit';
   
  $confirm_question = format_plural(count($records), 
			            'Are you sure you want to delete this item?',
			            'Are you sure you want to delete these items?');
  return confirm_form($form,
					$confirm_question,
					'info_management/data/process/1', t('This action cannot be undone.'),
					t('Delete'), t('Cancel')); 
}

function info_management_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    _record_delete_multiple(array_keys($form_state['values']['records']));
    $count = count($form_state['values']['records']);
    watchdog('info_management', 'Deleted @count records.', array('@count' => $count));
    drupal_set_message(format_plural($count, 'Delete 1 record.', 'Delete @count posts.'));
  }
  $form_state['redirect'] = 'info_management/data/process/1';
}

/**
 * Delete record by id
 */
function _record_delete_multiple($keys) {
  try { 
   foreach ($keys as $id) { 
	 db_delete('info_management')
	   ->condition('id', $id, '=')
	   ->execute();
   }
  }
  catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('node', $e);
	throw $e;
  }
}

/**
 *  Mass update or just update one 
 */
function info_management_mass_or_one_update($form, &$form_state, $arg0) {

  drupal_set_message('mass or one update3');
//  var_dump($form_state);
//  var_dump($form);
  if ($arg0 == 'ids') {
    $form['update'] = info_management_mass_update_form($form, $form_state, $arg0);
//    $form['#submit'][] = 'info_management_mass_update_form_submit';
  }
  elseif ($arg0 != null) {
    $form['update'] = info_management_update_one_form($form, $form_state, $arg0);
//    $form['#submit'][] = 'info_management_update_one_form_submit';     
  }
  else { 
    $_SESSION['record_mass_update'] = array(); 
    $form['update'] = info_management_mass_update_form($form, $form_state, $arg0);
//    $form['#submit'][] = 'info_management_update_one_form_submit';     
  }  
  return $form;  
}

/**
 * Make update form.
 */
function info_management_update_one_form($form, &$form_state, $arg0) {
  try {
	$id = $arg0;
	
	//Use Database API to retrieve records.
	$query = db_select('info_management', 'n')
	 ->fields('n', array('id', 'first_name', 'last_name', 'email', 'col_industry'))
	 ->condition('id', $arg0, '=')
	 ->orderBy('id', 'DESC'); //Most recent first.
		 
	$result = $query->execute();  
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	$txn->rollback();
	// Log the exception to watchdog.
	watchdog_exception('type', $e);
  }
  
  foreach ($result as $row){
	$form['description'] = array(
	  '#type' => 'item',
	  '#title' => t('Data Update for the ID# ' . $row->id . ' record'),
	);
	// This is the form elements.
	$form['update'] = array(
	  '#type' => 'fieldset',
  //	  '#title' => t('Personal Information'),
	   // Make the fieldset collapsible.
  //	  '#collapsible' => TRUE,
  //	  '#collapsed' => FALSE,
	); 
	$form['update']['id'] = array(
	  '#type' => 'hidden',
	  '#title' => t('ID'),
	  '#value' => $row->id,
	  '#readonly' => 'readonly',
	);	
	$form['update']['first_name'] = array(
	  '#type' => 'textfield',
	  '#title' => t('First Name in English: '),
	  '#size' => 50,
	  '#maxlength' => 50,
	  '#default_value' => $row->first_name,
	);
	$form['update']['last_name'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Last Name in English: '),
	  '#size' => 50,
	  '#maxlength' => 50,
	  '#default_value' => $row->last_name,
	);
	$form['update']['email'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Email: '),
	  '#size' => 50,
	  '#maxlength' => 100,
	  '#default_value' => $row->email,
	);	
	$form['update']['col_industry'] = array(
	  '#type' => 'select',
	  '#title' => 'Industry: ',
	  '#options' => info_management_subtable('info_management_industry', 'i_id'),
	  '#default_value' => $row->col_industry,
	);
    $form['update']['actions']['submit'] = array(
	  '#type' => 'submit',
	  '#value' => 'Submit',
	  '#submit' => array('info_management_update_one_form_submit'),
	  '#attributes' => array(
		'class' => array('info-management-update-submit'),
	  ),
	);  
	$form['update']['actions']['delete'] = array(
	  '#type' => 'submit',
	  '#value' => 'Delete',
	  '#submit' => array('info_management_delete_one_submit'),
	  '#attributes' => array(
		'class' => array('info-management-update-delete'),
	  ),
	);
  };

  return $form; 
}

/**
 * Update data.
 */
function info_management_update_one_form_submit($form, &$form_state) {
  drupal_set_message('update one');
  try { 
	$id = $form_state['values']['id'];
	$id_updated = db_update('info_management') // Table name no longer needs {}
	 ->fields(array(
	  'first_name' => $form_state['values']['first_name'],
	  'last_name' => $form_state['values']['last_name'],
	  'email' => $form_state['values']['email'],
	  'col_industry' => $form_state['values']['col_industry'],
	))
	 ->condition('id', $id, '=')
	 ->execute();
	
	drupal_set_message(t('The No. ' .  $id. ' record has been updated.'));
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($id_updated)  
	  $txn->rollback();
	// Log the exception to watchdog.
	watchdog_exception('type', $e);
  }
}

/**
 * Make mass update form.
 */
function info_management_mass_update_form($form, &$form_state, $arg0) {
  // Initial $_SESSION['record_mass_update']  to null 
//  if ($arg0 == 'empty') {
  //  $_SESSION['record_mass_update'] = array();
//  }
  // The record ids checked in display window for "mass update" option
  $session = isset($_SESSION['record_mass_update']) ? $_SESSION['record_mass_update'] : array();
  
  $session_temp = implode(',', $session);
  $options_industry = info_management_industry_table_id_to_name('info_management_industry');
    
  $form['description'] = array(
    '#type' => 'item',
//  '#title' => t('For the records with IDs: ' . $session_temp),
//	'#title_display' => 'invisible',
  );   
  if (!empty($session)) {
    $form['description']['#title'] = t('For the records with IDs: ' . $session_temp);
  }
  else {	
	$form['description']['#title'] = 'Batch Option';
	$form['description']['#title_display'] = 'invisible';
  }
  
  // This is the form elements.
  $form['mass_update'] = array(
    '#type' => 'fieldset',
  );
  $form['mass_update']['field_name'] = array(
    '#type' => 'select',
    '#title' => t('Which field to update?'),
  );  
  
  $filters = info_management_filters();
  foreach ($filters as $key => $value) {
    if($key != 'id') 
	  $form['mass_update']['field_name']['#options'][$key]  = $value['#title'];
  }	  
  	  
  $form['mass_update']['old_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Old Value:'),
	'#size' => 60,
    '#maxlength' => 60,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array(
		  array('value' => 'first_name'), 
		  array('value' => 'last_name'), 
		  array('value' => 'email'),
		),
	  ),
	), 
  );  
  $form['mass_update']['old_industry_value'] = array(
    '#type' => 'select',
    '#title' => t('Old Value:'),
	'#options' => $options_industry,
//	'#default_value' => 1,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array('value' => 'col_industry')
	  ),
	 ),
  );  
  
  $form['mass_update']['new_industry_value'] = array(
    '#type' => 'select',
    '#title' => t('New Value:'),
	'#options' => $options_industry,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array('value' => 'col_industry')
	  ),
	 ),
  ); 
  $form['mass_update']['new_value'] = array(
    '#type' => 'textfield',
    '#title' => t('New Value:'),
	'#size' => 60,
    '#maxlength' => 60,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array(
		  array('value' => 'first_name'), 
		  array('value' => 'last_name'), 
		  array('value' => 'email'),
		),
	  ),
	), 
  );
  $form['actions']['reset'] = array( 
    '#markup' => '<input class="info-management-mass-reset-submit form-submit" type="reset" value="' . t("Reset"). '"/>',  
  );
  $form['action']['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
	'#submit' => array('info_management_mass_update_form_submit'),
	'#attributes' => array(
		'class' => array('info-management-mass-update-submit'),
	),
  );   
  
  return $form; 
}

/**
 * Process info_management_mass_update form submissions.
 *  
 * Update the selected records.
 */   
function info_management_mass_update_form_submit($form, &$form_state) { 
  drupal_set_message('helper');
  // The record ids checked in display window for "mass update" option
  $session = isset($_SESSION['record_mass_update']) ? $_SESSION['record_mass_update'] : array();
  $field_name = $form_state['values']['field_name'];

  if ($field_name == 'col_industry') {
	$new_value = trim($form_state['values']['new_industry_value']);
	$old_value = trim($form_state['values']['old_industry_value']);
  } 
  else {
	$new_value = trim($form_state['values']['new_value']);
	$old_value = trim($form_state['values']['old_value']);
  }
  if (empty($session)) {
	$query = db_select('info_management', 'n')
	  ->fields('n', array('id'))
	  ->condition($field_name, $old_value, '=')
	  ->execute();
	$id_sum = $query->rowCount();
	$id_array = array();
	$id_array = $query->fetchAllAssoc('id');
	$id_array = array_keys($id_array);
  }  
  else {
    $id_array = $session;
  } 
    
  // We use batch processing to prevent timeout when updating a large number
  // of data
  if ($id_sum > 1) {
    $operations = array();
	$operations[] = array('_data_mass_update_process', array($field_name, $new_value, $old_value, $id_array));
 	$batch = array(
	  'operations' => $operations,
	  'finished' => '_data_mass_update_batch_finished',  
	  'title' => t('Processing'),
	  // We use a single multi-pass operation, so the default
	  // 'Remaining x of y operations' message will be confusing here.
	  'init_message' => t('The batch is starting.'),
	  'progress_message' => '',
	  'error_message' => t('The update has encountered an error.'),
	  // The operations do not live in the .module file, so we need to 
	  // tell the batch engine which file to load before calling them.
	  'file' => drupal_get_path('module', 'info_management') . '/process_data.inc', 
	);  	
	batch_set($batch);
  }
  else {
	foreach ($id_array as $id) {
	  _data_mass_update_helper($field_name, $new_value, $old_value, $id);
    }
    drupal_set_message(t('The update has been performed.'));
  }
}    

/**
 * Data Mass Update Batch operation
 */
function _data_mass_update_process($field_name, $new_value, $old_value, $id_array, &$context) {
  if(empty($context['sandbox'])) {
    $context['sandbox'] = array();
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($id_array);
    $context['sandbox']['id_array'] = $id_array;
  }
  // Process data by groups of 5.
  $count = min(2, count($context['sandbox']['id_array']));
  for ($i = 1; $i <= $count; $i++) {
    // For each id, reset the values
    $iid = array_shift($context['sandbox']['id_array']);
	drupal_set_message($field_name . $new_value . $old_value . $id);
 	_data_mass_update_helper($field_name, $new_value, $old_value, $iid);
    
    // Store result for post-processing in the finished callback.
    $context['results'][] = l(count($context['sandbox']['id_array']), $iid);

    // Update our progress information.
    $context['sandbox']['progress']++;
	// Optional message displayed under the progressbar.
    // $context['sandbox']['message'] = t('Loading "@iid"', array('@iid' => $iid));
  }  
  
  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] >= $context['sandbox']['max'];
  }
}

/**
 * Data Mass Update - helper function
 */
function _data_mass_update_helper($field_name, $new_value, $old_value, $id) {
  drupal_set_message($field_name . $new_value . $old_value . $id . 'helper');
  try { 
    $id_updated = db_update('info_management');	
	$id_updated->condition($field_name, $old_value, '=');
    $id_updated->condition('id', $id, '=');   
    $id_updated->fields(array($field_name => $new_value)); 	
	$affected_rows = $id_updated->execute();	
    drupal_set_message(t('The <i>' .  $field_name . ' </i>Field has been updated. <i>' . $old_value . '</i> is changed to <i>' . $new_value . '</i>.'));
	drupal_set_message(t($affected_rows . ' rows were affected'));
	return true;
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($id_updated)
	  $txn->rollback();
	// Log the exception to watchdog.
	watchdog_exception('type', $e);
  }
}

/**
 * Batch 'finished' callback.
 */
function _data_mass_update_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The update has been performed.')); 
  }
  else {
    // An error occurred.
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
    $message = format_plural(count($results), '1 item successfully processed:', '@count item successfully processed:');
    $message .= theme('item_list', array('items' => $results));
    drupal_set_message($message);
  }   
}

function info_management_subtable($arg0, $arg1) {
  $options = array();
  try {
    $results = db_select($arg0, 'n')
	  ->fields('n')
	  ->orderBy($arg1, 'DESC')
	  ->execute();
    foreach ($results as $row) {
 	  $options[$row->i_id] = $row->i_e_name . '/' . $row->i_c_name;
 	}	   	
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($results)
	  $txn->rollback();
	// Log the exception to watchdog.
	watchdog_exception('type', $e);    
  }
  return $options;
}

function info_management_industry_table_id_to_name($table, $arg0 = null) {
  $options = array();
  try {
    $query = db_select($table, 'n')
	  ->fields('n', array('i_id', 'i_c_name', 'i_e_name'));
	if ($arg0 != null) 
	  $query->condition('i_id', $arg0, '=');
	
	$results = $query->execute();
    foreach ($results as $row) {
 	  $options[$row->i_id] = $row->i_c_name . '/' . $row->i_e_name;
 	}	   	
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($results)
	  $txn->rollback();
	// Log the exception to watchdog.
	watchdog_exception('type', $e);    
  }
  return $options;
}

function info_management_country_table_id_to_name($arg0) {
  $options = array();
  try {
    $results = db_select($arg0, 'n')
	  ->fields('n')
	  ->condition('i_id', $arg0, '=')
	  ->execute()
	  ->fetchAll();
    foreach ($results as $row) {
 	  $options[$row->i_id] = $row->i_c_name . '/' . $row->i_e_name;
 	}	   	
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($results)
	  $txn->rollback();
	// Log the exception to watchdog.
	watchdog_exception('type', $e);    
  }
  return $options;
}