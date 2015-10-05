<?php

/**
  * @file
  * Page callbacks to administrate country table
  */ 
  
/**
  * Generate the country add form array
  *
  * @see info_management_country_form()
  */
function info_management_country_input_form($form, &$form_state) {
  // This is the form elements.
  $form['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data Input'),
	'#attributes' => array('class' => array('container-inline')),
  ); 
  $form['name']['c_e_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name in English:'),
	'#size' => 20,
    '#maxlength' => 20,
  ); 
  $form['name']['c_c_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name in Chinese:'),
	'#size' => 20,
    '#maxlength' => 20,
  ); 
  $form['name']['actions']['reset'] = array( 
    '#markup' => '<input class="info-management-country-reset-submit form-submit" type="reset" value="' . t("Reset"). '"/>',  
  );
  $form['name']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
	'#attributes' => array(
	   'class' => array('info-management-country-input-submit'),
	),
  );
  return $form;
}

/**
  * Submit handler for input_data().
  */
function info_management_country_input_form_submit($form, &$form_state) {
  try {
	$id = db_insert('info_management_country')
	  ->fields(array(
		'c_e_name' => check_plain($form_state['values']['c_e_name']),
		'c_c_name' => check_plain($form_state['values']['c_c_name']),
	  ))
	  ->execute();
	  
	  drupal_set_message(t('The form has been submitted. Name in English is "@e_name", Name in Chinese is "@c_name"',
		array(
		  '@e_name' => $form_state['values']['c_e_name'],
		  '@c_name' => $form_state['values']['c_c_name'],
		)
	  ));  
	}
  catch (Exception $e) {
	  // Something went wrong somewhere, so roll back now.
	  if ($id)
		$id->rollback();
	  // Log the exception to watchdog.
	  watchdog_exception('info_management_country_input', $e);
  }
}

/**
  * Menu callback: content management.
  */
function info_management_country_form($form, &$form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return info_management_country_delete_confirm($form, $form_state, $form_state['values']['id']);
  }
  $form['input'] = info_management_country_input_form();
  $form['#submit'][] = 'info_management_country_input_form_submit'; 
  $form['management'] = info_management_country_data();  
  return $form;
}

/**
  * Menu callback: update the info_management_country table.
  */
function info_management_country_update_form($form, &$form_state, $arg0) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return info_management_country_delete_confirm($form, $form_state, $arg0);
  } 
  $form['input'] = info_management_country_update_one_form($form, $form_state, $arg0);
  $form['#submit'][] = 'info_management_country_update_one_form_submit'; 
  $form['management'] = info_management_country_data();
  return $form;
}

/**
  * Form builder: Builds the data management overview.
  */  
function info_management_country_data() {  
  $user_access =  1; //user_access('Access content for the Information Management module');
 	
  // Build the sortable table header  
  $header = array(
    'c_id' => array('data' => t('ID'), 'field' => 'n.c_id', 'sort' => 'desc'),  
	'c_e_name' => array('data' => 'Name in English', 'field' => 'n.c_e_name'),
	'c_c_name' => array('data' => 'Name in Chinese', 'field' => 'n.c_c_name'),
	'operation' =>  t('Operation'), 
  );
  
  // Use Database API to retrieve records.
  $max_num = variable_get('info_management_max', 20);
  $query = db_select('info_management_country', 'n')->extend('PagerDefault')->extend('TableSort');

  $results = $query
     ->fields('n', array('c_id', 'c_e_name', 'c_c_name'))
     ->limit($max_num)
	 ->orderByHeader($header)
     ->addTag('info_management_country access')
     ->execute();

  // Prepare the list of records
  // $destination = drupal_get_destination();
  
  // Iterate over the resultset and format them.
  $rows = array();
  foreach ($results as $row){
//    $rows[$row->c_id] = array('id' => $row->id);
    $rows[$row->c_id] = array('c_id' => array('data' => $row->c_id, 'class' => array('info-management-column-i-id')));
	$rows[$row->c_id]['c_e_name'] = array('data' => $row->c_e_name, 'class' => array('info-management-column-i-e-name'));
	$rows[$row->c_id]['c_c_name'] = array('data' => $row->c_c_name, 'class' => array('info-management-column-i-c-name'));	
	// Build a list for all the accessible options for the current node
	$operations = array();
    if ($user_access) {
	  $operations['delete'] = array(
		'title' => t('Delete'),
		'href' => 'info_management/data/process/country/1/' . $row->c_id,
 //		'query' => $row->id,
	  );
	  $operations['update'] = array(
		'title' => t('Update'),
		'href' => 'info_management/data/process/country/2/' . $row->c_id,
 //		'query' => $row->id,
	  );
  	}
	$rows[$row->c_id]['operation'] = array();
	if(count($operations) > 1) {
	  // Render an unordered list of operations links
	  $rows[$row->c_id]['operation'] = array(
		'data' => array(
		  '#theme' => 'links',
		  '#links' => $operations,
		  '#attributes' => array('class' => array('links', 'inline')),
		),
		'class' => array('info-management-country-column-operation'),
	  );  
	}
	elseif(!empty($operations)) {
	  // Render the first and only operation as a link
	  $link = reset($operations);
	  $rows[$row->c_id]['operation'] = array(
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
  // '#attributes' => array('class' => array('info-management-table')),
  );
  // Attach the pager theme.
  $form['pager'] = array('#markup' => theme('pager'));
  return $form; 
}

/** 
 * Menu callback -- ask for confirmation of record deletion
 */
function info_management_country_delete_confirm($form, &$form_state, $arg0) {
 
  $form = array();
  $form['id'] = array('#type' => 'value', '#value' => $arg0);
  
  return confirm_form($form,
	t('Are you sure you want to delete No. %id record?', array('%id' => $arg0)),
	'info_management/data/process/country/',
	t('This action cannot be undone.'),
	t('Delete'), t('Cancel')
  );
}

function info_management_country_delete_confirm_submit($form, &$form_state, $arg0) {
  if ($form_state['values']['confirm']) {
    _record_delete_one(array($form_state['values']['id']));
	watchdog('info_management', 'Deleted @id record from country table.', array('@id' => $form_state['values']['id']));
	drupal_set_message('%id has been deleted.', array('%id' => $form_state['values']['id']));
  }
  $form_state['redirect'] = 'info_management/data/process/country/';
}

/**
 * Delete record by id
 */
function _record_delete_one($keys) {
  // Transaction
  $transaction = db_transaction();
  try { 
    db_query('LOCK TABLES {info_management_country} WRITE, {info_management} WRITE;');
    foreach ($keys as $id) { 
	  $result = db_delete('info_management_country')
	    ->condition('c_id', $id, '=')
	    ->execute();
      $id_updated = db_update('info_management')  
        ->fields(array('col_country' => '1'))
        ->condition('col_country', $id, '=')
        ->execute();
    }
	db_query('UNLOCK TABLES;');
  }
  catch (Exception $e) {
    $transaction->rollback();
	watchdog_exception('_record_delete_one', $e);
	throw $e;
  }
  // End of Transaction
}
 
/**
 * Make update form.
 */
function info_management_country_update_one_form($form, $form_state, $arg0) {
  try {
	// Use Database API to retrieve records.
	$query = db_select('info_management_country', 'n')
	  ->fields('n', array('c_id', 'c_e_name', 'c_c_name'))
	  ->condition('c_id', $arg0, '=')
	  ->orderBy('c_id', 'DESC'); // Most recent first.
		 
	$result = $query->execute();  
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($query)
	  $query->rollback();
	// Log the exception to watchdog.
	watchdog_exception('info_management_country_select', $e);
  }
  
  foreach ($result as $row){
	$form['description'] = array(
	  '#type' => 'item',
	  '#title' => t('Data Update for the ID# ' . $row->c_id . ' record'),
	);
	// This is the form elements.
	$form['update'] = array(
	  '#type' => 'fieldset',
  //	  '#title' => t('Personal Information'),
  //      Make the fieldset collapsible.
  //	  '#collapsible' => TRUE,
  //	  '#collapsed' => FALSE,
      '#attributes' => array('class' => array('container-inline')),
	); 
	$form['update']['id'] = array(
	  '#type' => 'hidden',
	  '#title' => t('ID'),
	  '#value' => $row->c_id,
	  '#readonly' => 'readonly',
	);	
	$form['update']['c_e_name'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Name in English:'),
	  '#size' => 20,
	  '#maxlength' => 20,
	  '#default_value' => $row->c_e_name,
	);
	$form['update']['c_c_name'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Name in Chinese:'),
	  '#size' => 20,
	  '#maxlength' => 20,
	  '#default_value' => $row->c_c_name,
	);
	$form['update']['actions']['reset'] = array( 
	  '#markup' => '<input class="info-management-country-reset-submit form-submit" type="reset" value="' . t("Reset"). '"/>',  
	);
	$form['update']['actions']['submit'] = array(
	  '#type' => 'submit',
	  '#value' => t('Update'),
	  '#attributes' => array(
		'class' => array('info-management-country-update-submit'),
	  ),
	);
  };

  return $form; 
}

/**
 * Update data.
 */
function info_management_country_update_one_form_submit($form, &$form_state) {
  try { 
    $id = $form_state['values']['id'];
    $id_updated = db_update('info_management_country') // Table name no longer needs {}
      ->fields(array(
	    'c_e_name' => $form_state['values']['c_e_name'],
	    'c_c_name' => $form_state['values']['c_c_name'],
      ))
      ->condition('c_id', $id, '=')
      ->execute();
	
     drupal_set_message(t('The No. ' .  $id. ' record has been updated.'));
  }
  catch (Exception $e) {
    // Something went wrong somewhere, so roll back now.
    if ($id_updated) $id_updated->rollback();
    // Log the exception to watchdog.
    watchdog_exception('info_management_country_update', $e);
  }
}
