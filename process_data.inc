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
  $form['contact'] = array(
    '#type' => 'fieldset',
    //'#title' => t('Personal Information'),
	// Make the fieldset collapsible.
    //'#collapsible' => TRUE,
    //'#collapsed' => FALSE,
  ); 
  $form['contact']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name in English:'),
	'#size' => 50,
    '#maxlength' => 50,
  );
  $form['contact']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name in English:'),
	'#size' => 50,
    '#maxlength' => 50,
  );  
  $form['contact']['chinese_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Chinese name:'),
	'#size' => 50,
    '#maxlength' => 50,
  );
  $form['contact']['company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company:'),
	'#size' => 100,
    '#maxlength' => 200,
  );
  $form['contact']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title:'),
	'#size' => 100,
    '#maxlength' => 200,
  );
  $form['contact']['website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website:'),
	'#size' => 50,
    '#maxlength' => 50,
  ); 
  $form['contact']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email:'),
	'#size' => 50,
    '#maxlength' => 50,
  ); 
  $form['contact']['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address:'),
	'#size' => 100,
    '#maxlength' => 200,
  ); 
  
  $form['contact']['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City:'),
	'#size' => 50,
    '#maxlength' => 50,
  ); 
  $form['contact']['province'] = array(
    '#type' => 'textfield',
    '#title' => t('Province:'),
	'#size' => 50,
    '#maxlength' => 100,
  ); 
  $form['contact']['postcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Postcode:'),
	'#size' => 20,
    '#maxlength' => 20,
  ); 
  
  $form['contact']['mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile:'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $form['contact']['tel_work'] = array(
    '#type' => 'textfield',
    '#title' => t('Tel in office:'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $form['contact']['tel_home'] = array(
    '#type' => 'textfield',
    '#title' => t('Tel at home:'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $form['contact']['fax'] = array(
    '#type' => 'textfield',
    '#title' => t('Fax:'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $form['contact']['note'] = array(
    '#type' => 'textarea',
    '#title' => t('Note:'),
	'#description' => t('No more than 1000 words please!'),
	'#cols' => 50,
  );     
  
  $form['contact']['col_industry'] = array(
    '#type' => 'select',
    '#title' => t('Industry:'),
	'#options' => info_management_subtable('info_management_industry', 'i_id'),
  );  
  $form['contact']['col_country'] = array(
    '#type' => 'select',
    '#title' => t('Country:'),
	'#options' => info_management_subtable('info_management_country', 'c_id'),
  );
  $form['contact']['col_source'] = array(
    '#type' => 'select',
    '#title' => t('Source:'),
	'#options' => info_management_subtable('info_management_source', 's_id'),
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
  $transaction = db_transaction();
  try {
	$id = db_insert('info_management')
	->fields(array(
	  'first_name' => check_plain($form_state['values']['first_name']),
	  'last_name' => check_plain($form_state['values']['last_name']),
      'chinese_name' => check_plain($form_state['values']['chinese_name']),
      'company' => check_plain($form_state['values']['company']),
      'title' => check_plain($form_state['values']['title']),
      'website' => check_plain($form_state['values']['website']),
	  'email' => check_plain($form_state['values']['email']),
      'address' => check_plain($form_state['values']['address']),
      'city' => check_plain($form_state['values']['city']),
	  'province' => check_plain($form_state['values']['province']),
      'postcode' => check_plain($form_state['values']['postcode']),
      'mobile' => check_plain($form_state['values']['mobile']),
	  'tel_work' => check_plain($form_state['values']['tel_work']),
      'tel_home' => check_plain($form_state['values']['tel_home']),
	  'fax' => check_plain($form_state['values']['fax']),
	  'col_country' => check_plain($form_state['values']['col_country']),
	  'col_industry' => check_plain($form_state['values']['col_industry']),
	  'col_source' => check_plain($form_state['values']['col_source']),
      'note' => check_plain($form_state['values']['note']),
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
	$transaction->rollback();
	// Log the exception to watchdog.
	watchdog_exception('info_management_input', $e);
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
  // '#markup' => t('SHOW ONLY ITEMS WHERE'),
  
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
  $form['filter']['id'] = array(
    '#type' => 'textfield',
    '#title' => t('ID:'),
	'#size' => 10,
    '#maxlength' => 10,
  );
  $form['filter']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name in English:'),
	'#size' => 50,
    '#maxlength' => 50,
  );
  $form['filter']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name in English:'),
	'#size' => 50,
    '#maxlength' => 50,
  );  
  $form['filter']['chinese_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Chinese name:'),
	'#size' => 50,
    '#maxlength' => 50,
  );
  $form['filter']['company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company:'),
	'#size' => 100,
    '#maxlength' => 200,
  );
  $form['filter']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title:'),
	'#size' => 100,
    '#maxlength' => 200,
  );
  $form['filter']['website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website:'),
	'#size' => 50,
    '#maxlength' => 50,
  ); 
  $form['filter']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email:'),
	'#size' => 50,
    '#maxlength' => 50,
  ); 
  $form['filter']['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address:'),
	'#size' => 100,
    '#maxlength' => 200,
  ); 
  
  $form['filter']['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City:'),
	'#size' => 50,
    '#maxlength' => 50,
  ); 
  $form['filter']['province'] = array(
    '#type' => 'textfield',
    '#title' => t('Province:'),
	'#size' => 50,
    '#maxlength' => 100,
  ); 
  $form['filter']['postcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Postcode:'),
	'#size' => 20,
    '#maxlength' => 20,
  ); 
  
  $form['filter']['mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile:'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $form['filter']['tel_work'] = array(
    '#type' => 'textfield',
    '#title' => t('Tel in office:'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $form['filter']['tel_home'] = array(
    '#type' => 'textfield',
    '#title' => t('Tel at home:'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $form['filter']['fax'] = array(
    '#type' => 'textfield',
    '#title' => t('Fax:'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $form['filter']['note'] = array(
    '#type' => 'textarea',
    '#title' => t('Note:'),
	'#cols' => 50,
  ); 
  
  $form['filter']['col_country'] = array(
    '#type' => 'select',
    '#title' => t('Country:'),
  );
  $form['filter']['col_country']['#options']['0'] = "---------"; 
  $form['filter']['col_country']['#options'] += info_management_subtable('info_management_country', 'c_id');
  $form['filter']['col_country']['#default_value'] = 0; 
  
  $form['filter']['col_industry'] = array(
    '#type' => 'select',
    '#title' => t('Industry:'),
/*	'#options' => info_management_subtable('info_management_industry', 'i_id'), */
  );
  $form['filter']['col_industry']['#options']['0'] = "---------"; 
  $form['filter']['col_industry']['#options'] += info_management_subtable('info_management_industry', 'i_id');
  $form['filter']['col_industry']['#default_value'] = 0; 
  
  $form['filter']['col_source'] = array(
    '#type' => 'select',
    '#title' => t('Source:'),
  );
  $form['filter']['col_source']['#options']['0'] = "---------"; 
  $form['filter']['col_source']['#options'] += info_management_subtable('info_management_source', 's_id');
  $form['filter']['col_source']['#default_value'] = 0; 
  
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
		if (trim($form_state['values'][$key]) != null) {
		  $_SESSION['record_overview_filter'][$key] = $form_state['values'][$key];
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
  
 // $col_checked = info_management_checked_column($columns); 
  $data_table_column_name = array();
  $data_table_column_name = info_management_check_table_column_name();
  $data_table_column_checked = info_management_checked_column($data_table_column_name[0]);
  $data_table_column_checked = drupal_map_assoc($data_table_column_checked);
  
//  drupal_set_message(t('Submitting values: @values', array('@values' => var_export($data_table_column_checked, TRUE))));
 
  $user_access =  1; // user_access('Access content for the Information Management module');
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
  $header_full = array(
    'id' => array('data' => t('ID'), 'field' => 'n.id', 'sort' => 'desc'),  
	'first_name' => array('data' => 'First Name', 'field' => 'n.first_name'),
	'last_name' => array('data' => 'Last Name', 'field' => 'n.last_name'),
	'chinese_name' => array('data' => t('Chinese Name'), 'field' => 'n.chinese_name'),
	'company' => array('data' => t('Company'), 'field' => 'n.website'),
	'title' => array('data' => t('Title'), 'field' => 'n.title'),
	'website' => array('data' => t('Website'), 'field' => 'n.website'),
	'email' => array('data' => t('Email'), 'field' => 'n.email'),
	'address' => array('data' => t('Address'), 'field' => 'n.address'),
	'city' => array('data' => t('City'), 'field' => 'n.city'),
	'province' => array('data' => t('Province'), 'field' => 'n.province'),
	'postcode' => array('data' => t('Postcode'), 'field' => 'n.postcode'),
	'mobile' => array('data' => t('Mobile'), 'field' => 'n.mobile'),
	'tel_work' => array('data' => t('Tel at work'), 'field' => 'n.tel_work'),
	'tel_home' => array('data' => t('Tel at home'), 'field' => 'n.tel_home'),  
	'fax' => array('data' => t('Fax'), 'field' => 'n.fax'), 
	'col_country' => array('data' => t('Country'), 'field' => 'n.col_country'), 
	'col_industry' => array('data' => t('Industry'), 'field' => 'n.col_industry'),
	'col_source' => array('data' => t('Source'), 'field' => 'n.col_source'),
    'note' => array('data' => t('Note'), 'field' => 'n.note'),
	'operation' =>  t('Operation'), 
  );
  $header = array_intersect_key($header_full, $data_table_column_checked);
//  drupal_set_message(t('Submitting values: @values', array('@values' => var_export($header_full, TRUE))));
//  drupal_set_message(t('Submitting values: @values', array('@values' => var_export($data_table_column_checked, TRUE))));
//  drupal_set_message(t('Submitting values: @values', array('@values' => var_export($header, TRUE))));
  
  // Use Database API to retrieve records.
  $max_num = variable_get('info_management_max', 3);
  $query = db_select('info_management', 'n')->extend('PagerDefault')->extend('TableSort');
  _record_build_filter_query($query);
  $query->leftJoin('info_management_country', 'coun', 'n.col_country = coun.c_id');
  $query->leftJoin('info_management_industry', 'indu', 'n.col_industry = indu.i_id');
  $query->leftJoin('info_management_source', 'sour', 'n.col_source = sour.s_id');  

  $results = $query
	->fields('n', array('id', 'first_name', 'last_name', 'chinese_name', 'company', 'title', 
			'website', 'email', 'address', 'city', 'province', 'postcode', 'mobile', 'tel_work', 'tel_home',
			'fax', 'note'))
	->fields('coun', array('c_id', 'c_c_name', 'c_e_name'))
	->fields('indu', array('i_id', 'i_c_name', 'i_e_name'))
	->fields('sour', array('s_id', 's_c_name', 's_e_name'))
	->limit($max_num)
	->orderByHeader($header)
	->addTag('info_management access')
	->execute();
 
  // Prepare the list of records  
  // Iterate over the resultset and format them.
  $rows = array();
  foreach ($results as $row){
    // $rows[$row->id] = array('id' => $row->id);
	$rows[$row->id] = array('id' => array('data' => $row->id, 'class' => array('info-management-column-id')));
	$rows[$row->id]['first_name'] = array('data' => $row->first_name, 'class' => array('info-management-column-first-name'));
	$rows[$row->id]['last_name'] = array('data' => $row->last_name, 'class' => array('info-management-column-last-name'));
	$rows[$row->id]['chinese_name'] = array('data' => $row->chinese_name, 'class' => array('info-management-column-chinese-name'));
	$rows[$row->id]['company'] = array('data' => $row->company, 'class' => array('info-management-column-company')); 
	$rows[$row->id]['title'] = array('data' => $row->title, 'class' => array('info-management-column-title')); 
	$rows[$row->id]['website'] = array('data' => $row->website, 'class' => array('info-management-column-website'));
	$rows[$row->id]['email'] = array('data' => $row->email, 'class' => array('info-management-column-email')); 
	$rows[$row->id]['address'] = array('data' => $row->address, 'class' => array('info-management-column-address')); 
	$rows[$row->id]['city'] = array('data' => $row->city, 'class' => array('info-management-column-city'));  
	$rows[$row->id]['province'] = array('data' => $row->province, 'class' => array('info-management-column-province'));
	$rows[$row->id]['postcode'] = array('data' => $row->postcode, 'class' => array('info-management-column-postcode'));  
	$rows[$row->id]['mobile'] = array('data' => $row->mobile, 'class' => array('info-management-column-mobile')); 
	$rows[$row->id]['tel_work'] = array('data' => $row->tel_work, 'class' => array('info-management-column-tel-work'));  
	$rows[$row->id]['tel_home'] = array('data' => $row->tel_home, 'class' => array('info-management-column-tel_home'));
	$rows[$row->id]['fax'] = array('data' => $row->fax, 'class' => array('info-management-column-fax'));  
	$rows[$row->id]['note'] = array('data' => $row->note, 'class' => array('info-management-column-note'));
	$rows[$row->id]['col_country'] = array('data' => $row->c_e_name . '/' . $row->c_c_name, 'class' => array('info-management-column-country')); 
	$rows[$row->id]['col_industry'] = array('data' => $row->i_e_name . '/' . $row->i_c_name, 'class' => array('info-management-column-industry')); 
	$rows[$row->id]['col_source'] = array('data' => $row->s_e_name . '/' . $row->s_c_name, 'class' => array('info-management-column-source')); 
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
    '#prefix' => '<div id="info-management-contacts-table">',
	'#suffix' => '</div>',
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
    '#title' => t('First Name in English'),
	'#size' => 50,
    '#maxlength' => 50,
  );
  $filters['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name in English'),
	'#size' => 50,
    '#maxlength' => 50,
  );  
  $filters['chinese_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Chinese Name'),
	'#size' => 50,
    '#maxlength' => 50,
  );
  $filters['company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company'),
	'#size' => 100,
    '#maxlength' => 200,
  );
  $filters['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
	'#size' => 100,
    '#maxlength' => 200,
  );
  $filters['website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website'),
	'#size' => 50,
    '#maxlength' => 50,
  ); 
  $filters['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
	'#size' => 50,
    '#maxlength' => 50,
  ); 
  $filters['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
	'#size' => 100,
    '#maxlength' => 200,
  ); 
  
  $filters['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
	'#size' => 50,
    '#maxlength' => 50,
  ); 
  $filters['province'] = array(
    '#type' => 'textfield',
    '#title' => t('Province'),
	'#size' => 50,
    '#maxlength' => 100,
  ); 
  $filters['postcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Postcode'),
	'#size' => 20,
    '#maxlength' => 20,
  );   
  $filters['mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $filters['tel_work'] = array(
    '#type' => 'textfield',
    '#title' => t('Tel in office'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $filters['tel_home'] = array(
    '#type' => 'textfield',
    '#title' => t('Tel at home'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $filters['fax'] = array(
    '#type' => 'textfield',
    '#title' => t('Fax'),
	'#size' => 20,
    '#maxlength' => 20,
  );  
  $filters['note'] = array(
    '#type' => 'textarea',
    '#title' => t('Note'),
	'#cols' => 50,
  );  
  $filters['col_country'] = array(
	'#type' => 'textfield',
	'#title' => t('Country'),
  );
  $filters['col_industry'] = array(
	'#type' => 'textfield',
	'#title' => t('Industry'),
  );      
  $filters['col_source'] = array(
	'#type' => 'textfield',
	'#title' => t('Source'),
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
	  switch ($filter_key) {
		case 'col_country':
		  if ($filter_value != '0') 
		    $query->condition($filter_key, $filter_value, '=');
		  break;
		case 'col_industry':
		  if ($filter_value != '0') 
		    $query->condition($filter_key, $filter_value, '=');
		  break;
		case 'col_source':	
		  if ($filter_value != '0') 
		    $query->condition($filter_key, $filter_value, '=');	
		  break;
		default: 
		  $query->condition($filter_key, '%' . db_like($filter_value) . '%', 'LIKE');
	  }      
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
	if ($function == 'info_management_mass_update') {
	  $_SESSION['record_mass_update'] = $records;
	  $form_state['redirect'] = 'info_management/data/process/3/ids';
	}
  }
  else {   
    // We need to rebuild the form to go to a second step. For example, to
    // show the confirmation form for the deletion of nodes.
    $form_state['rebuild'] = TRUE;
  }
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
	 $result = db_delete('info_management')
	   ->condition('id', $id, '=')
	   ->execute();
   }
  }
  catch (Exception $e) {
	if ($result) $result->rollback();
	watchdog_exception('_record_delete_multiple', $e);
	throw $e;
  }
}

/**
 *  Mass update or just update one 
 */
function info_management_mass_or_one_update($form, &$form_state, $arg0) {
  if ($arg0 == 'ids') {
    $form['update'] = info_management_mass_update_form($form, $form_state, $arg0);
  }
  elseif ($arg0 != null) {
    $form['update'] = info_management_update_one_form($form, $form_state, $arg0);
  }
  else { 
    $_SESSION['record_mass_update'] = array(); 
    $form['update'] = info_management_mass_update_form($form, $form_state, $arg0);
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
	  ->fields('n', array('id', 'first_name', 'last_name', 'email', 'col_country', 'col_industry', 'col_source'))
	  ->condition('id', $arg0, '=')
	  ->orderBy('id', 'DESC'); // Most recent first.
		 
	$result = $query->execute();  
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($query) $query->rollback();
	// Log the exception to watchdog.
	watchdog_exception('info_management_update_one_form', $e);
  }
  
  foreach ($result as $row){
	$form['description'] = array(
	  '#type' => 'item',
	  '#title' => t('Data Update for the ID# ' . $row->id . ' record'),
	);
	// This is the form elements.
	$form['update'] = array(
	  '#type' => 'fieldset',
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
	$form['update']['col_country'] = array(
	  '#type' => 'select',
	  '#title' => 'Country: ',
	  '#options' => info_management_subtable('info_management_country', 'c_id'),
	  '#default_value' => $row->col_country,
	);	
	$form['update']['col_industry'] = array(
	  '#type' => 'select',
	  '#title' => 'Industry: ',
	  '#options' => info_management_subtable('info_management_industry', 'i_id'),
	  '#default_value' => $row->col_industry,
	);
	$form['update']['col_source'] = array(
	  '#type' => 'select',
	  '#title' => 'Source: ',
	  '#options' => info_management_subtable('info_management_source', 's_id'),
	  '#default_value' => $row->col_source,
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
	  'first_name' => check_plain($form_state['values']['first_name']),
	  'last_name' => check_plain($form_state['values']['last_name']),
	  'chinese_name' => check_plain($form_state['values']['chinese_name']),
	  'company' => check_plain($form_state['values']['company']),
	  'title' => check_plain($form_state['values']['title']),
	  'website' => check_plain($form_state['values']['website']),
	  'email' => check_plain($form_state['values']['email']),
	  'address' => check_plain($form_state['values']['address']),
	  'city' => check_plain($form_state['values']['city']),
	  'province' => check_plain($form_state['values']['province']),
	  'postcode' => check_plain($form_state['values']['postcode']),
	  'tel_work' => check_plain($form_state['values']['tel_work']),
	  'tel_home' => check_plain($form_state['values']['tel_home']),
	  'fax' => check_plain($form_state['values']['fax']),
	  'note' => check_plain($form_state['values']['note']),
	  'col_country' => check_plain($form_state['values']['col_country']),
	  'col_industry' => check_plain($form_state['values']['col_industry']),
	  'col_source' => check_plain($form_state['values']['col_source']),
	))
	 ->condition('id', $id, '=')
	 ->execute();
	
	drupal_set_message(t('The No. ' .  $id. ' record has been updated.'));
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($id_updated)  
	  $id_updated->rollback();
	// Log the exception to watchdog.
	watchdog_exception('info_management_update_one', $e);
  }
}

/**
 * Make mass update form.
 */
function info_management_mass_update_form($form, &$form_state, $arg0) {
  // The record ids checked in display window for "mass update" option
  $session = isset($_SESSION['record_mass_update']) ? $_SESSION['record_mass_update'] : array();
  
  $session_temp = implode(',', $session);
  $options_country = info_management_country_table_id_to_name('info_management_country');
  $options_industry = info_management_industry_table_id_to_name('info_management_industry');
  $options_source = info_management_source_table_id_to_name('info_management_source');
    
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
  $form['mass_update']['old_value_small'] = array(
    '#type' => 'textfield',
    '#title' => t('Old Value:'),
	'#size' => 20,
    '#maxlength' => 20,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array(
		  array('value' => 'first_name'), 
		  array('value' => 'last_name'), 
		  array('value' => 'chinese_name'),
		  array('value' => 'postcode'),
		  array('value' => 'mobile'),
		  array('value' => 'tel_work'),
		  array('value' => 'tel_home'),
		  array('value' => 'fax'),
		),
	  ),
	), 
  );  
  $form['mass_update']['new_value_small'] = array(
    '#type' => 'textfield',
    '#title' => t('New Value:'),
	'#size' => 20,
    '#maxlength' => 20,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array(
		  array('value' => 'first_name'), 
		  array('value' => 'last_name'), 
		  array('value' => 'chinese_name'),
		  array('value' => 'postcode'),
		  array('value' => 'mobile'),
		  array('value' => 'tel_work'),
		  array('value' => 'tel_home'),
		  array('value' => 'fax'),
		),
	  ),
	), 
  );    	  
  $form['mass_update']['old_value_middle'] = array(
    '#type' => 'textfield',
    '#title' => t('Old Value:'),
	'#size' => 50,
    '#maxlength' => 50,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array(
		  array('value' => 'website'), 
		  array('value' => 'email'), 
		  array('value' => 'city'),
		  array('value' => 'province'),
		),
	  ),
	), 
  );  
  $form['mass_update']['new_value_middle'] = array(
    '#type' => 'textfield',
    '#title' => t('New Value:'),
	'#size' => 50,
    '#maxlength' => 50,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array(
		  array('value' => 'website'), 
		  array('value' => 'email'), 
		  array('value' => 'city'),
		  array('value' => 'province'),
		),
	  ),
	), 
  );
  $form['mass_update']['old_value_large'] = array(
    '#type' => 'textfield',
    '#title' => t('Old Value:'),
	'#size' => 100,
    '#maxlength' => 200,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array(
		  array('value' => 'company'), 
		  array('value' => 'title'), 
		  array('value' => 'address'),
		),
	  ),
	), 
  );  
  $form['mass_update']['new_value_large'] = array(
    '#type' => 'textfield',
    '#title' => t('New Value:'),
	'#size' => 100,
    '#maxlength' => 200,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array(
		  array('value' => 'company'), 
		  array('value' => 'title'), 
		  array('value' => 'address'),
		),
	  ),
	), 
  );  
  $form['mass_update']['old_value_note'] = array(
    '#type' => 'textfield',
    '#title' => t('Old Value:'),
	'#description' => t('No more than 1000 words please!'),
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array(
		  array('value' => 'note'), 
		),
	  ),
	), 
  );
  $form['mass_update']['new_value_note'] = array(
    '#type' => 'textarea',
    '#title' => t('New Value:'),
    '#description' => t('No more than 1000 words please!'),
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array(
		  array('value' => 'note'), 
		),
	  ),
	), 
  );   
  $form['mass_update']['old_country_value'] = array(
    '#type' => 'select',
    '#title' => t('Old Value:'),
	'#options' => $options_country,
//	'#default_value' => 1,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array('value' => 'col_country')
	  ),
	 ),
  );  
  $form['mass_update']['new_country_value'] = array(
    '#type' => 'select',
    '#title' => t('New Value:'),
	'#options' => $options_country,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array('value' => 'col_country')
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
    '#title' => t('new Value:'),
	'#options' => $options_industry,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array('value' => 'col_industry')
	  ),
	 ),
  ); 
  $form['mass_update']['old_source_value'] = array(
    '#type' => 'select',
    '#title' => t('Old Value:'),
	'#options' => $options_source,
//	'#default_value' => 1,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array('value' => 'col_source')
	  ),
	 ),
  );  
  $form['mass_update']['new_source_value'] = array(
    '#type' => 'select',
    '#title' => t('New Value:'),
	'#options' => $options_source,
	'#states' => array(
	  'visible' => array(
		':input[name="field_name"]' => array('value' => 'col_source')
	  ),
	 ),
  ); 
  $form['actions']['reset'] = array( 
    '#markup' => '<input class="info-management-mass-reset-submit form-submit" type="reset" value="' . t("Reset"). '"/>',  
  );
  $form['actions']['update'] = array(
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
  // The record ids checked in display window for "mass update" option
  $session = isset($_SESSION['record_mass_update']) ? $_SESSION['record_mass_update'] : array();
  $field_name = $form_state['values']['field_name'];
  
  if ($field_name == 'website' || $field_name == 'email' ||  $field_name == 'city' ||  $field_name == 'province') {
    $field_name_sw =  'col_middle'; 
  }   
  else if ($field_name == 'company' || $field_name == 'title' ||  $field_name == 'address') {
    $field_name_sw =  'col_large'; 
  } 
  else {
    $field_name_sw = $field_name;
  }
  switch ($field_name_sw) {
  	case 'col_middle':
	  $new_value = trim(check_plain($form_state['values']['new_value_middle']));
	  $old_value = trim(check_plain($form_state['values']['old_value_middle']));	
	  break;
	case 'col_large':
	  $new_value = trim(check_plain($form_state['values']['new_value_large']));
	  $old_value = trim(check_plain($form_state['values']['old_value_large']));	
	  break;
	case 'note':
	  $new_value = trim(check_plain($form_state['values']['new_value_note']));
	  $old_value = trim(check_plain($form_state['values']['old_value_note']));	
	  break;
    case 'col_country': 
	  $new_value = trim(check_plain($form_state['values']['new_country_value']));
	  $old_value = trim(check_plain($form_state['values']['old_country_value']));	
	  break;
	case 'col_industry':
	  $new_value = trim(check_plain($form_state['values']['new_industry_value']));
	  $old_value = trim(check_plain($form_state['values']['old_industry_value']));	
	  break;
	case 'col_source':
	  $new_value = trim(check_plain($form_state['values']['new_source_value']));
	  $old_value = trim(check_plain($form_state['values']['old_source_value']));	
	  break;
	default:
	  $new_value = trim(check_plain($form_state['values']['new_value_small']));
	  $old_value = trim(check_plain($form_state['values']['old_value_small']));	  
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
  if ($id_sum > 5) {   
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
  $count = min(5, count($context['sandbox']['id_array']));
  for ($i = 1; $i <= $count; $i++) {
    // For each id, reset the values
    $iid = array_shift($context['sandbox']['id_array']);
	drupal_set_message($field_name . $new_value . $old_value . $id);
 	_data_mass_update_helper($field_name, $new_value, $old_value, $iid);
    
    // Store result for post-processing in the finished callback.
    $context['results'][] = l(count($context['sandbox']['id_array']), $iid);

    // Update our progress information.
    $context['sandbox']['progress']++;
	// Optional message displayed under the progress bar.
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
	  $id_updated->rollback();
	// Log the exception to watchdog.
	watchdog_exception('_data_mass_update_helper', $e);
  }
}

/**
 * Batch 'finished' callback.
 */
function _data_mass_update_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The update has been performed!')); 
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
  $id = $arg0[16] . '_id';
  $field_e = $arg0[16] . '_e_name';
  $field_c = $arg0[16] . '_c_name';
  $options = array();
  try {
    $results = db_select($arg0, 'n')
	  ->fields('n')
	  ->orderBy($arg1, 'DESC')
	  ->execute();
    foreach ($results as $row) {
 	  $options[$row->$id] = $row->$field_e . '/' . $row->$field_c;
 	}	   	
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($results)
	  $results->rollback();
	// Log the exception to watchdog.
	watchdog_exception('info_management_subtable', $e);    
  }
  return $options;
}

/**
 * Get the countries' names in English and Chinese from the "info_management_country" table
 */ 
function info_management_country_table_id_to_name($table, $arg0 = null) {
  $options = array();
  try {
    $query = db_select($table, 'n')
	  ->fields('n', array('c_id', 'c_c_name', 'c_e_name'));
	if ($arg0 != null) 
	  $query->condition('c_id', $arg0, '=');
	
	$results = $query->execute();
    foreach ($results as $row) {
 	  $options[$row->c_id] = $row->c_c_name . '/' . $row->c_e_name;
 	}	   	
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($results)
	  $results->rollback();
	// Log the exception to watchdog.
	watchdog_exception('info_management_country_table_id_to_name', $e);    
  }
  return $options;
}

/**
 * Get the industries' names in English and Chinese from the "info_management_industry" table
 */ 
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
	  $results->rollback();
	// Log the exception to watchdog.
	watchdog_exception('info_management_industry_table_id_to_name', $e);    
  }
  return $options;
}

/**
 * Get the sources' names in English and Chinese from the "info_management_source" table
 */ 
function info_management_source_table_id_to_name($table, $arg0 = null) {
  $options = array();
  try {
    $query = db_select($table, 'n')
	  ->fields('n', array('s_id', 's_c_name', 's_e_name'));
	if ($arg0 != null) 
	  $query->condition('s_id', $arg0, '=');
	
	$results = $query->execute();
    foreach ($results as $row) {
 	  $options[$row->s_id] = $row->s_c_name . '/' . $row->s_e_name;
 	}	   	
  }
  catch (Exception $e) {
	// Something went wrong somewhere, so roll back now.
	if ($results)
	  $results->rollback();
	// Log the exception to watchdog.
	watchdog_exception('info_management_source_table_id_to_name', $e);    
  }
  return $options;
}
