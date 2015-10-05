<?php

/**
  * @file
  * Page callbacks to administrate source table
  */ 
 
/**
 * The form to make update.
 */
function info_management_check_update_form($form, $form_state) {
  $form = array();
  $columns = array(); 
//  $columns_value = array();
  $col_checked = array();  
  
  $cols = info_management_check_table_column_name();
  $col_checked = info_management_checked_column($cols[1]);

  // The checked field will be shown in the display page  
  $form['check']['fields'] = array(
	'#title' => t('The checked Fields will be shown in the data display:'),
	'#type' => 'checkboxes',
	'#description' => t(''),
	'#options' => $cols[1],
	'#default_value' => $col_checked,
  );
  $form['check']['actions']['submit'] = array(
	'#type' => 'submit',
	'#value' => t('Update'),
	'#attributes' => array(
	   'class' => array('info-management-check-update-submit'),
	),
  ); 
  return $form; 
}

/**
 * Update data.
 */
function info_management_check_update_form_submit($form, &$form_state) {
//  drupal_set_message(t('Submitting values: @values', array('@values' => var_export($form_state['values'], TRUE))));  
  $temp_fields = array();   
  foreach ($form_state['values']['fields'] as $field_name => $field_value) {
    if (!empty($field_value))
      $temp_fields[$field_name] = 1; 
    else
	  $temp_fields[$field_name] = 0;
  }  
  try { 
	$id_updated = db_update('info_management_field_check') // Table name no longer needs {}
	  ->fields($temp_fields)
	  ->execute();
	
    drupal_set_message(t('The record has been updated.'));
  }
  catch (Exception $e) {
    // Something went wrong somewhere, so roll back now.
    if ($id_updated) $id_updated->rollback();
    // Log the exception to watchdog.
    watchdog_exception('info_management_field_check_update', $e);
  }
}
