<?php
// $Id$
/**
* @file
* Audio Blog VoIP Scripts and helper functions
*/

/**
 * Implementation of hook_voipscript_get_script_names()
 */
function voipcommerce_voipscript_get_script_names() {
  $script_names[] = 'voipcommerce_order_status_script';
  return $script_names;
}

/**
 * Implementation of hook_voipscript_load_script()
 */
function voipcommerce_voipscript_load_script($script_name, $options=NULL) {

  $script = NULL;
  switch ($script_name) {
    default:
      break;
    case 'voipcommerce_order_status_script':
      //type the invoice number and get information about a current order status
      $script = new VoipScript('voipcommerce_order_status_script');
      $script->addSay(v('Welcome to the VoIP Commerce shop.'));
      $script->addLabel('main_menu');
      $options_menu = v('For order status, dial 1. For customer support, dial 2. For hours of operation, dial 3. To hang up, dial the star key. ');
      $input_options = array(
        '1' => 'status',
        '2' => 'customer support',
        '3' => 'hours',
        '*' => 'hang up',
        'i' => 'hang up',
        't' => 'hang up'
      );
      $invalid_msg = v('Invalid option selected.');
      $script->addRunIvrMenu($options_menu, $input_options, $invalid_msg);
      $script->addGoto('%ivr_option_selected');
      $script->addLabel('status');
      $prompt = v("Type your invoice number to get information about a current order status. When done press the pound key.");
      $timeout = 5;
      $end_key = '#';
      //Use NULL to allow for variable number of digits:
      $num_digits = NULL; 
      $script->addGetInput($prompt, $num_digits, $end_key, $timeout);
      $script->addSet('order_status','^_voipcommerce_order_status_callback(%input_digits)');
      $script->addSay('%order_status');
      $script->addGoto('main_menu');
      
      $script->addLabel('customer support');
      $script->addSay(v('Customer support department'));
      $script->addGoto('main_menu');
    
      $script->addLabel('hours');
      $script->addSay(v('Our office is open Monday to Friday from 9am to 5pm.'));
      $script->addGoto('main_menu');
      $script->addLabel('hang up');
      $script->addSay(v('Thanks so much for calling our office. Bye bye.'));
      $script->addHangup();
      break;  
  }

  return $script;
}

function _voipcommerce_order_status_callback($order_id) {
  $order = uc_order_load($order_id);
  if (isset($order->order_status)) {
    $order_state = uc_order_state_data($order->order_status,'title');
    $say = "Your order is marked as ".$order_state;
  }
  else {
    $say = "Sorry, order number ".$order_id." is not found.";
  }
  return $say;
}