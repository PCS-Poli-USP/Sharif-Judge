<?php
/**
 * Sharif Judge online judge
 * @file Install.php
 * @author Mohammad Javad Naderi <mjnaderi@gmail.com>
 */
defined('BASEPATH') OR exit('No direct script access allowed');

class Install extends CI_Controller
{

	public function __construct()
	{
		parent::__construct();
		$this->load->helper('string');
	}


	// ------------------------------------------------------------------------


	public function index()
	{


		if ($this->db->table_exists('sessions'))
			show_error('Sharif Judge is already installed.');

		$this->form_validation->set_rules('username', 'username', 'required|min_length[3]|max_length[20]|alpha_numeric|lowercase');
		$this->form_validation->set_rules('email', 'email', 'required|max_length[40]|valid_email|lowercase');
		$this->form_validation->set_rules('password', 'password', 'required|min_length[6]|max_length[200]');
		$this->form_validation->set_rules('password_again', 'password confirmation', 'required|matches[password]');

		$data['installed'] = FALSE;

		if ($this->form_validation->run()) {

			$DATETIME = 'DATETIME';
			if ($this->db->dbdriver === 'postgre')
				$DATETIME = 'TIMESTAMP';


			$this->load->dbforge();


			// Use InnoDB engine for MySql database
			if ($this->db->dbdriver === 'mysql' || $this->db->dbdriver === 'mysqli')
				$this->db->query('SET storage_engine=InnoDB;');

			// Creating Tables:
			// sessions, submissions, assignments, notifications, problems, queue, scoreboard, settings, users


			// create table 'sessions'
			$fields = array(
				'session_id'    => array('type' => 'VARCHAR', 'constraint' => 40, 'default' => '0'),
				'ip_address'    => array('type' => 'VARCHAR', 'constraint' => 45, 'default' => '0'),
				'user_agent'    => array('type' => 'VARCHAR', 'constraint' => 120),
				'last_activity' => array('type' => 'INT', 'constraint' => 10, 'unsigned' => TRUE, 'default' => '0'),
				'user_data'     => array('type' => 'TEXT'),
			);
			$this->dbforge->add_field($fields);
			$this->dbforge->add_key('session_id', TRUE); // PRIMARY KEY
			$this->dbforge->add_key('last_activity');
			if ( ! $this->dbforge->create_table('sessions', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('sessions'));



			// create table 'submissions'
			$fields = array(
				'submit_id'     => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE),
				'username'      => array('type' => 'VARCHAR', 'constraint' => 20),
				'assignment'    => array('type' => 'SMALLINT', 'constraint' => 4, 'unsigned' => TRUE),
				'problem'       => array('type' => 'SMALLINT', 'constraint' => 4, 'unsigned' => TRUE),
				'is_final'      => array('type' => 'TINYINT', 'constraint' => 1, 'default' => 0),
				'time'          => array('type' => $DATETIME),
				'status'        => array('type' => 'VARCHAR', 'constraint' => 100),
				'pre_score'     => array('type' => 'INT', 'constraint' => 11),
				'coefficient'   => array('type' => 'VARCHAR', 'constraint' => 6),
				'file_name'     => array('type' => 'VARCHAR', 'constraint' => 30),
				'main_file_name'=> array('type' => 'VARCHAR', 'constraint' => 30),
				'file_type'     => array('type' => 'VARCHAR', 'constraint' => 6),
			);
			$this->dbforge->add_field($fields);
			$this->dbforge->add_key(array('assignment', 'submit_id'));
			if ( ! $this->dbforge->create_table('submissions', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('submissions'));



			// create table 'assignments'
			$fields = array(
				'id'            => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE, 'auto_increment' => TRUE),
				'name'          => array('type' => 'VARCHAR', 'constraint' => 50, 'default' => ''),
				'problems'      => array('type' => 'SMALLINT', 'constraint' => 4, 'unsigned' => TRUE),
				'total_submits' => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE),
				'open'          => array('type' => 'TINYINT', 'constraint' => 1),
				'scoreboard'    => array('type' => 'TINYINT', 'constraint' => 1),
				'javaexceptions'=> array('type' => 'TINYINT', 'constraint' => 1),
				'description'   => array('type' => 'TEXT', 'default' => ''),
				'start_time'    => array('type' => $DATETIME),
				'finish_time'   => array('type' => $DATETIME),
				'extra_time'    => array('type' => 'INT', 'constraint' => 11),
				'late_rule'     => array('type' => 'TEXT'),
				'participants'  => array('type' => 'TEXT', 'default' => ''),
				'moss_update'   => array('type' => 'VARCHAR', 'constraint' => 30, 'default' => 'Never'),
			);
			$this->dbforge->add_field($fields);
			$this->dbforge->add_key('id', TRUE); // PRIMARY KEY
			if ( ! $this->dbforge->create_table('assignments', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('assignments'));


			// create table 'notifications'
			$fields = array(
				'id'            => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE, 'auto_increment' => TRUE),
				'title'         => array('type' => 'VARCHAR', 'constraint' => 200, 'default' => ''),
				'text'          => array('type' => 'TEXT', 'default' => ''),
				'time'          => array('type' => $DATETIME),
			);
			$this->dbforge->add_field($fields);
			$this->dbforge->add_key('id', TRUE); // PRIMARY KEY
			if ( ! $this->dbforge->create_table('notifications', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('notifications'));



			// create table 'problems'
			$fields = array(
				'assignment'        => array('type' => 'SMALLINT', 'constraint' => 4, 'unsigned' => TRUE),
				'id'                => array('type' => 'SMALLINT', 'constraint' => 4, 'unsigned' => TRUE),
				'name'              => array('type' => 'VARCHAR', 'constraint' => 50, 'default' => ''),
				'score'             => array('type' => 'INT', 'constraint' => 11),
				'is_upload_only'    => array('type' => 'TINYINT', 'constraint' => 1, 'default' => '0'),
				'c_time_limit'      => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE, 'default' => 500),
				'python_time_limit' => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE, 'default' => 1500),
				'java_time_limit'   => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE, 'default' => 2000),
				'memory_limit'      => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE, 'default' => 50000),
				'allowed_languages' => array('type' => 'TEXT', 'default' => ''),
				'diff_cmd'          => array('type' => 'VARCHAR', 'constraint' => 20, 'default' => 'diff'),
				'diff_arg'          => array('type' => 'VARCHAR', 'constraint' => 20, 'default' => '-bB'),
				'weight' 			=> array('type' => 'INT'),
				'static_analysis'	=> array('type' => 'INT'),
			);
			$this->dbforge->add_field($fields);
			$this->dbforge->add_key(array('assignment', 'id')); // BTREE KEY
			$this->dbforge->add_key('assignment', TRUE); //COMPOSITE PRIMARY KEY
			$this->dbforge->add_key('id', TRUE); //COMPOSITE PRIMARY KEY
			if ( ! $this->dbforge->create_table('problems', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('problems'));


			// create table 'static_analysis'
			$fields = array(
				'assignment'       				=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'problem'						=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'public_methods'               	=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'public_methods_max'         	=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'auxiliary_classes'         	=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'auxiliary_classes_max'   		=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'unnecessary_attributes'     	=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'unnecessary_attributes_max'	=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'lower_camel_case'  			=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'lower_camel_case_max'    		=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'code_quality'					=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'code_quality_max'       		=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'duplicated_code'         		=> array('type' => 'SMALLINT', 'unsigned' => TRUE),
				'static_analysis_weight' 		=> array('type' => 'INT', 'unsigned' => TRUE),
			);
			$this->dbforge->add_key('assignment', TRUE); //COMPOSITE PRIMARY KEY
			$this->dbforge->add_key('problem', TRUE); //COMPOSITE PRIMARY KEY
			$this->dbforge->add_field($fields);
			if ( ! $this->dbforge->create_table('static_analysis', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('static_analysis'));
			// ADD FOREING KEY TO shj_problems TABLE
			$this->db->query(
				"ALTER TABLE {$this->db->dbprefix('static_analysis')}
				 ADD CONSTRAINT {$this->db->dbprefix('static_analysis_problem_fkey')} FOREIGN KEY(assignment, problem) REFERENCES {$this->db->dbprefix('problems')}(assignment, id) ON DELETE CASCADE;"
			);


			// create table 'queue'
			$fields = array(
				'id'                => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE, 'auto_increment' => TRUE),
				'submit_id'         => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE),
				'username'          => array('type' => 'VARCHAR', 'constraint' => 20),
				'assignment'        => array('type' => 'SMALLINT', 'constraint' => 4, 'unsigned' => TRUE),
				'problem'           => array('type' => 'SMALLINT', 'constraint' => 4, 'unsigned' => TRUE),
				'type'              => array('type' => 'VARCHAR', 'constraint' => 8),
			);
			$this->dbforge->add_key('id', TRUE); // PRIMARY KEY
			$this->dbforge->add_field($fields);
			if ( ! $this->dbforge->create_table('queue', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('queue'));
			//Add UNIQUE (submit_id, username, assignment, problem) constraint
			$this->db->query(
				"ALTER TABLE {$this->db->dbprefix('queue')}
				 ADD CONSTRAINT {$this->db->dbprefix('suap_unique')} UNIQUE (submit_id, username, assignment, problem);"
			);



			// create table 'scoreboard'
			$fields = array(
				'assignment'        => array('type' => 'SMALLINT', 'constraint' => 4, 'unsigned' => TRUE),
				'scoreboard'        => array('type' => 'TEXT', 'default' => ''),
			);
			$this->dbforge->add_field($fields);
			$this->dbforge->add_key('assignment');
			if ( ! $this->dbforge->create_table('scoreboard', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('scoreboard'));



			// create table 'settings'
			$fields = array(
				'shj_key'        => array('type' => 'VARCHAR', 'constraint' => 50),
				'shj_value'      => array('type' => 'TEXT', 'default' => ''),
			);
			$this->dbforge->add_field($fields);
			$this->dbforge->add_key('shj_key');
			if ( ! $this->dbforge->create_table('settings', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('settings'));



			// insert default settings to table 'settings'
			$result = $this->db->insert_batch('settings', array(
				array('shj_key' => 'timezone',               'shj_value' => 'Asia/Tehran'),
				array('shj_key' => 'tester_path',            'shj_value' => '/home/shj/tester'),
				array('shj_key' => 'assignments_root',       'shj_value' => '/home/shj/assignments'),
				array('shj_key' => 'file_size_limit',        'shj_value' => '50'),
				array('shj_key' => 'output_size_limit',      'shj_value' => '1024'),
				array('shj_key' => 'queue_is_working',       'shj_value' => '0'),
				array('shj_key' => 'default_late_rule',      'shj_value' => "/* \n * Put coefficient (from 100) in variable \$coefficient.\n * You can use variables \$extra_time and \$delay.\n * \$extra_time is the total extra time given to users\n * (in seconds) and \$delay is number of seconds passed\n * from finish time (can be negative).\n *  In this example, \$extra_time is 172800 (2 days):\n */\n\nif (\$delay<=0)\n  // no delay\n  \$coefficient = 100;\n\nelseif (\$delay<=3600)\n  // delay less than 1 hour\n  \$coefficient = ceil(100-((30*\$delay)/3600));\n\nelseif (\$delay<=86400)\n  // delay more than 1 hour and less than 1 day\n  \$coefficient = 70;\n\nelseif ((\$delay-86400)<=3600)\n  // delay less than 1 hour in second day\n  \$coefficient = ceil(70-((20*(\$delay-86400))/3600));\n\nelseif ((\$delay-86400)<=86400)\n  // delay more than 1 hour in second day\n  \$coefficient = 50;\n\nelseif (\$delay > \$extra_time)\n  // too late\n  \$coefficient = 0;"),
				array('shj_key' => 'enable_easysandbox',     'shj_value' => '1'),
				array('shj_key' => 'enable_c_shield',        'shj_value' => '1'),
				array('shj_key' => 'enable_cpp_shield',      'shj_value' => '1'),
				array('shj_key' => 'enable_scoreboard',      'shj_value' => '1'),
				array('shj_key' => 'enable_py2_shield',      'shj_value' => '1'),
				array('shj_key' => 'enable_py3_shield',      'shj_value' => '1'),
				array('shj_key' => 'enable_java_policy',     'shj_value' => '1'),
				array('shj_key' => 'enable_log',             'shj_value' => '1'),
				array('shj_key' => 'submit_penalty',         'shj_value' => '300'),
				array('shj_key' => 'final_grade',            'shj_value' => '1'),
				array('shj_key' => 'enable_registration',    'shj_value' => '0'),
				array('shj_key' => 'registration_code',      'shj_value' => '0'),
				array('shj_key' => 'mail_from',              'shj_value' => 'shj@example.com'),
				array('shj_key' => 'mail_from_name',         'shj_value' => 'Sharif Judge'),
				array('shj_key' => 'reset_password_mail',    'shj_value' => "<p>\nSomeone requested a password reset for your Sharif Judge account at {SITE_URL}.\n</p>\n<p>\nTo change your password, visit this link:\n</p>\n<p>\n<a href=\"{RESET_LINK}\">Reset Password</a>\n</p>\n<p>\nThe link is valid for {VALID_TIME}. If you don't want to change your password, just ignore this email.\n</p>"),
				array('shj_key' => 'add_user_mail',          'shj_value' => "<p>\nHello! You are registered in Sharif Judge at {SITE_URL} as {ROLE}.\n</p>\n<p>\nYour username: {USERNAME}\n</p>\n<p>\nYour password: {PASSWORD}\n</p>\n<p>\nYou can log in at <a href=\"{LOGIN_URL}\">{LOGIN_URL}</a>\n</p>"),
				array('shj_key' => 'moss_userid',            'shj_value' => ''),
				array('shj_key' => 'results_per_page_all',   'shj_value' => '40'),
				array('shj_key' => 'results_per_page_final', 'shj_value' => '80'),
				array('shj_key' => 'week_start',             'shj_value' => '0'),
			));
			if ( ! $result)
				show_error("Error adding data to table ".$this->db->dbprefix('settings'));



			// create table 'users'
			$fields = array(
				'id'                  => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE, 'auto_increment' => TRUE),
				'username'            => array('type' => 'VARCHAR', 'constraint' => 20),
				'password'            => array('type' => 'VARCHAR', 'constraint' => 100),
				'display_name'        => array('type' => 'VARCHAR', 'constraint' => 40, 'default' => ''),
				'email'               => array('type' => 'VARCHAR', 'constraint' => 40),
				'role'                => array('type' => 'VARCHAR', 'constraint' => 20),
				'passchange_key'      => array('type' => 'VARCHAR', 'constraint' => 60, 'default' => ''),
				'passchange_time'     => array('type' => $DATETIME, 'null' => TRUE),
				'first_login_time'    => array('type' => $DATETIME, 'null' => TRUE),
				'last_login_time'     => array('type' => $DATETIME, 'null' => TRUE),
				'selected_assignment' => array('type' => 'SMALLINT', 'constraint' => 4, 'unsigned' => TRUE, 'default' => 0),
				'dashboard_widget_positions'   => array('type' => 'VARCHAR', 'constraint' => 500, 'default' => ''),
			);
			$this->dbforge->add_field($fields);
			$this->dbforge->add_key('id', TRUE); // PRIMARY KEY
			$this->dbforge->add_key('username'); // @todo is this needed?
			if ( ! $this->dbforge->create_table('users', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('users'));

			//create table 'classes'
			$fields = array(
				'id' => array('type' => 'INT', 'constraint' => 11, 'unsigned' => TRUE, 'auto_increment' => TRUE),
				'time_start' => array('type' => 'VARCHAR', 'constraint' => 5),
				'time_end' => array('type' => 'VARCHAR', 'constraint' => 5),
				'day' => array('type' => 'INT'),
				'classroom' => array('type' => 'VARCHAR', 'constraint' => 20),
				'class_name' => array('type' => 'VARCHAR', 'constraint' => 20),
			);
			$this->dbforge->add_field($fields);
			$this->dbforge->add_key('id', TRUE); // PRIMARY KEY
			if ( ! $this->dbforge->create_table('classes', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('classes'));

			//create table 'users_classes'
			$fields = array(
				'user_id' => array('type' => 'INT', 'constraint' => 11),
				'class_id' => array('type' => 'INT', 'constraint' => 11),
				'responsible' => array('type' => 'INT'),
			);
			$this->dbforge->add_field($fields);
			$this->dbforge->add_key('user_id', TRUE);
			$this->dbforge->add_key('class_id', TRUE);
			if ( ! $this->dbforge->create_table('users_classes', TRUE)) {
				show_error("Error creating database table ".$this->db->dbprefix('users_classes'));
			}


			$this->db->query(
				"ALTER TABLE {$this->db->dbprefix('users_classes')}
				 ADD CONSTRAINT {$this->db->dbprefix('user_classes_fkey')} FOREIGN KEY(user_id) REFERENCES {$this->db->dbprefix('users')}(id) ON DELETE CASCADE;"
			);



			$this->db->query(
				"ALTER TABLE {$this->db->dbprefix('users_classes')}
				 ADD CONSTRAINT {$this->db->dbprefix('classes_fkey')} FOREIGN KEY(class_id) REFERENCES {$this->db->dbprefix('classes')}(id) ON DELETE CASCADE;"
			);


			/*
			$this->db->query("ALTER TABLE 'users_classes' ADD FOREIGN KEY('class_id') REFERENCES 'classes'('id') ON DELETE CASCADE;");
			 */

			//create table 'assignments_classes'
			$fields = array(
				'assignment_id' => array('type' => 'INT', 'constraint' => 11),
				'class_id' => array('type' => 'INT', 'constraint' => 11, 'null' => TRUE),
			);
			$this->dbforge->add_field($fields);
			$this->dbforge->add_key('assignment_id');
			$this->dbforge->add_key('class_id');
			if ( ! $this->dbforge->create_table('assignments_classes', TRUE))
				show_error("Error creating database table ".$this->db->dbprefix('assignments_classes'));


			$this->db->query(
				"ALTER TABLE {$this->db->dbprefix('assignments_classes')}
				 ADD CONSTRAINT {$this->db->dbprefix('assignment_fkey')} FOREIGN KEY(assignment_id) REFERENCES {$this->db->dbprefix('assignments')}(id) ON DELETE CASCADE;"
			);


			$this->db->query(
				"ALTER TABLE {$this->db->dbprefix('assignments_classes')}
				 ADD CONSTRAINT {$this->db->dbprefix('class_fkey')} FOREIGN KEY(class_id) REFERENCES {$this->db->dbprefix('classes')}(id) ON DELETE CASCADE;"
			);

			// add admin user
			$this->user_model->add_user(
				$this->input->post('username'),
				$this->input->post('email'),
				$this->input->post('password'),
				'admin'
			);

			// Using a random string as encryption key
			$config_path = rtrim(APPPATH,'/').'/config/config.php';
			$config_content = file_get_contents($config_path);
			$random_key = random_string('alnum', 32);
			$res = @file_put_contents($config_path, str_replace($this->config->item('encryption_key'), $random_key, $config_content));
			if ($res === FALSE)
				$data['key_changed'] = FALSE;
			else
				$data['key_changed'] = TRUE;

			$data['installed'] = TRUE;
			$data['enc_key'] = $this->config->item('encryption_key');
			$data['random_key'] = random_string('alnum', 32);
		}

		$this->twig->display('pages/admin/install.twig', $data);

	}
}
