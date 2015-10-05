<?php

/**
  * @file
  * Page callbacks to administrate source table
  */ 
  
/**
  * Generate the source add form array
  *
  * @see info_management_source_form()
  */
function info_management_source_input_form($form, &$form_state) {
  // This is the form elements.
  $form['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data Input'),
	'#attributes' => array('class' => array('container-inline')),
  ); 
  $form['name']['s_e_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name in English:'),
	'#size' => 20,
    '#maxlength' => 20,
  ); 
  $form['name']['s_c_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name in Chinese:'),
	'#size' => 20,
    '#maxlength' => 20,
  ); 
  $form['name']['actions']['reset'] = array( 
    '#markup' => '<input class="info-management-source-reset-submit form-submit" type="reset" value="' . t("Reset"). '"/>',  
  );
  $form['name']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
	'#attributes' => array(
	   'class' => array('info-management-source-input-submit'),
	  ),
  );
  return $form;
}

/**
  * Submit handler for input_data().
  */
function info_management_source_input_form_submit($form, &$form_state) {
  try {
	$id = db_insert('info_management_source')
	  ->fields(array(
		's_e_name' => check_plain($form_state['values']['s_e_name']),
		's_c_name' => check_plain($form_state['values']['s_c_name']),
	  ))
	  ->execute();
	  
	  drupal_set_message(t('The form has been submitted. Name in English is "@e_name", Name in Chinese is "@c_name"',
		array(
		  '@e_name' => $form_state['values']['s_e_name'],
		  '@c_name' => $form_state['values']['s_c_name'],
		)
	  ));  
	}
  catch (Exception $e) {
	  // Something went wrong somewhere, so roll back now.
	  if ($id)
		$id->rollback();
	  // Log the exception to watchdog.
	  watchdog_exception('info_management_source_input', $e);
  }
}

/**
  * Menu callback: content management.
  */
function info_management_source_form($form, &$form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return info_management_source_delete_confirm($form, $form_state, $form_state['values']['id']);
  }
  $form['input'] = info_management_source_input_form();
  $form['#submit'][] = 'info_management_source_input_form_submit'; 
  $form['management'] = info_management_source_data();  
  return $form;
}

/**
  * Menu callback: update the info_management_source table.
  */
function info_management_source_update_form($form, &$form_state, $arg0) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return info_management_source_delete_confirm($form, $form_state, $arg0);
  } 
  $form['input'] = info_management_source_update_one_form($form, $form_state, $arg0);
  $form['#submit'][] = 'info_management_source_update_one_form_submit'; 
  $form['management'] = info_management_source_data();
  return $form;
}

/**
  * Form builder: Builds the data management overview.
  */  
function info_management_source_data() {  
  $user_access =  1; //user_access('Access content for the Information Management module');
 	
  // Build the sortable table header  
  $header = array(
    's_id' => array('data' => t('ID'), 'field' => 'n.s_id', 'sort' => 'desc'),  
	's_e_name' => array('data' => 'Name in English', 'field' => 'n.s_e_name'),
	's_c_name' => array('data' => 'Name in Chinese', 'field' => 'n.s_c_name'),
	'operation' =>  t('Operation'), 
  );
  
  // Use Database API to retrieve records.
  $max_num = variable_get('info_management_max', 20);
  $query = db_select('info_management_source', 'n')->extend('PagerDefault')->extend('TableSort');

  $results = $query
     ->fields('n', array('s_id', 's_e_name', 's_c_name'))
     ->limit($max_num)
	 ->orderByHeader($header)
     ->addTag('info_management_source access')
     ->execute();

  // Prepare the list of records
  // $destination = drupal_get_destination();
  
  // Iterate over the resultset and format them.
  $rows = array();
  foreach ($results as $row){
//    $rows[$row->s_id] = array('id' => $row->id);
    $rows[$row->s_id] = array('s_id' => array('data' => $row->s_id, 'class' => array('info-management-column-i-id')));
	$rows[$row->s_id]['s_e_name'] = array('data' => $row->s_e_name, 'class' => array('info-management-column-i-e-name'));
	$rows[$row->s_id]['s_c_name'] = array('data' => $row->s_c_name, 'class' => array('info-management-column-i-c-name'));	
	// Build a list for all the accessible options for the current node
	$operations = array();
    if ($user_access) {
	  $operations['delete'] = array(
		'title' => t('Delete'),
		'href' => 'info_management/data/process/source/1/' . $row->s_id,
 //		'query' => $row->id,
	  );
	  $operations['update'] = array(
		'title' => t('Update'),
		'href' => 'info_management/data/process/source/2/' . $row->s_id,
 //		'query' => $row->id,
	  );
  	}
	$rows[$row->s_id]['operation'] = array();
	if(count($operations) > 1) {
	  // Render an unordered list of operations links
	  $rows[$row->s_id]['operation'] = array(
		'data' => array(
		  '#theme' => 'links',
		  '#links' => $operations,
		  '#attributes' => array('class' => array('links', 'inline')),
		),
		'class' => array('info-management-source-column-operation'),
	  );  
	}
	elseif(!empty($operations)) {
	  // Render the first and only operation as a link
	  $link = reset($operations);
	  $rows[$row->s_id]['operation'] = array(
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
 * Menu callback -- ask for confirmation of record deletion
 */
function info_management_source_delete_confirm($form, &$form_state, $arg0) {
 
  $form = array();
  $form['id'] = array('#type' => 'value', '#value' => $arg0);
  
  return confirm_form($form,
	t('Are you sure you want to delete No. %id record?', array('%id' => $arg0)),
	'info_management/data/process/source/',
	t('This action cannot be undone.'),
	t('Delete'), t('Cancel')
  );
}

function info_management_source_delete_confirm_submit($form, &$form_state, $arg0) {
  if ($form_state['values']['confirm']) {
    _record_delete_one(array($form_state['values']['id']));
	watchdog('info_management', 'Deleted @id record from source table.', array('@id' => $form_state['values']['id']));
	drupal_set_message('%id has been deleted.', array('%id' => $form_state['values']['id']));
  }
  $form_state['redirect'] = 'info_management/data/process/source/';
}

/**
 * Delete record by id
 */
function _record_delete_one($keys) {
  // Transaction
  $transaction = db_transaction();
  try { 
    db_query('LOCK TABLES {info_management_source} WRITE, {info_management} WRITE;');
    foreach ($keys as $id) { 
	  $result = db_delete('info_management_source')
	    ->condition('c_id', $id, '=')
	    ->execute();
      $id_updated = db_update('info_management')  
        ->fields(array('col_source' => '1'))
        ->condition('col_source', $id, '=')
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
function info_management_source_update_one_form($form, $form_state, $arg0) {
  try {
	//Use Database API to retrieve records.
	$query = db_select('info_management_source', 'n')
	 ->fields('n', array('s_id', 's_e_name', 's_c_name'))
	 ->condition('s_id', $arg0, '=')
	 ->orderBy('s_id', 'DESC'); //Most recent first.
		 
	$result = $query->execute();  
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($query)
	  $query->rollback();
	// Log the exception to watchdog.
	watchdog_exception('info_management_source_select', $e);
  }
  
  foreach ($result as $row){
	$form['description'] = array(
	  '#type' => 'item',
	  '#title' => t('Data Update for the ID# ' . $row->s_id . ' record'),
	);
	// This is the form elements.
	$form['update'] = array(
	  '#type' => 'fieldset',
  //	  '#title' => t('Personal Information'),
	   // Make the fieldset collapsible.
  //	  '#collapsible' => TRUE,
  //	  '#collapsed' => FALSE,
      '#attributes' => array('class' => array('container-inline')),
	); 
	$form['update']['id'] = array(
	  '#type' => 'hidden',
	  '#title' => t('ID'),
	  '#value' => $row->s_id,
	  '#readonly' => 'readonly',
	);	
	$form['update']['s_e_name'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Name in English:'),
	  '#size' => 20,
	  '#maxlength' => 20,
	  '#default_value' => $row->s_e_name,
	);
	$form['update']['s_c_name'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Name in Chinese:'),
	  '#size' => 20,
	  '#maxlength' => 20,
	  '#default_value' => $row->s_c_name,
	);
	$form['update']['actions']['reset'] = array( 
	  '#markup' => '<input class="info-management-source-reset-submit form-submit" type="reset" value="' . t("Reset"). '"/>',  
	);
	$form['update']['actions']['submit'] = array(
	  '#type' => 'submit',
	  '#value' => t('Update'),
	  '#attributes' => array(
		 'class' => array('info-management-source-update-submit'),
		),
	);
  };

  return $form; 
}

/**
 * Update data.
 */
function info_management_source_update_one_form_submit($form, &$form_state) {

 try { 
   $id = $form_state['values']['id'];
   $id_updated = db_update('info_management_source') // Table name no longer needs {}
     ->fields(array(
	  's_e_name' => $form_state['values']['s_e_name'],
	  's_c_name' => $form_state['values']['s_c_name'],
    ))
     ->condition('s_id', $id, '=')
     ->execute();
	
    drupal_set_message(t('The No. ' .  $id. ' record has been updated.'));
  }
  catch (Exception $e) {
    // Something went wrong somewhere, so roll back now.
    if ($id_updated) $id_updated->rollback();
    // Log the exception to watchdog.
    watchdog_exception('info_management_source_update', $e);
  }
}
