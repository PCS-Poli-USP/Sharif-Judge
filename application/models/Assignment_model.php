<?php
/**
 * Sharif Judge online judge
 * @file Assignment_model.php
 * @author Mohammad Javad Naderi <mjnaderi@gmail.com>
 */
defined('BASEPATH') OR exit('No direct script access allowed');

class Assignment_model extends CI_Model
{

	public function __construct()
	{
		parent::__construct();
	}



	// ------------------------------------------------------------------------



	/**
	 * Add New Assignment to DB / Edit Existing Assignment
	 *
	 * @param $id
	 * @param bool $edit
	 * @return bool
	 */
	public function add_assignment($id, $edit = FALSE)
	{
		// Start Database Transaction
		$this->db->trans_start();

		$extra_items = explode('*', $this->input->post('extra_time'));
		$extra_time = 1;
		foreach($extra_items as $extra_item)
		{
			$extra_time *= $extra_item;
		}
		$assignment = array(
			'id' => $id,
			'name' => $this->input->post('assignment_name'),
			'problems' => $this->input->post('number_of_problems'),
			'total_submits' => 0,
			'open' => ($this->input->post('open')===NULL?0:1),
			'scoreboard' => ($this->input->post('scoreboard')===NULL?0:1),
			'javaexceptions' => ($this->input->post('javaexceptions')===NULL?0:1),
			'description' => '', /* todo */
			'start_time' => date('Y-m-d H:i:s', strtotime($this->input->post('start_time'))),
			'finish_time' => date('Y-m-d H:i:s', strtotime($this->input->post('finish_time'))),
			'extra_time' => $extra_time*60,
			'late_rule' => $this->input->post('late_rule'),
			'participants' => $this->input->post('participants'),
		);
		if($edit)
		{
			$before = $this->db->get_where('assignments', array('id'=>$id))->row_array();
			unset($assignment['total_submits']);
			$this->db->where('id', $id)->update('assignments', $assignment);
			// each time we edit an assignment, we should update coefficient of all submissions of that assignment
			if ($assignment['extra_time']!=$before['extra_time'] OR $assignment['start_time']!=$before['start_time'] OR $assignment['finish_time']!=$before['finish_time'] OR $assignment['late_rule']!=$before['late_rule'])
				$this->_update_coefficients($id, $assignment['extra_time'], $assignment['finish_time'], $assignment['late_rule']);
		}
		else
			$this->db->insert('assignments', $assignment);
	
		if($this->input->post("all_classes")){
				$assignments_classes = array(
					'assignment_id' => $id,
					'class_id' => NULL,
				);
				$this->db->insert('assignments_classes', $assignments_classes);
		}else{
			foreach ($this->input->post('classes') as $class_id){
				$assignments_classes = array(
					'assignment_id' => $id,
					'class_id' => $class_id,
				);
				$this->db->insert('assignments_classes', $assignments_classes);
			}
		}
			


		/* **** Adding problems to "problems" table **** */

		//And remove all previous static analysis too
		$this->db->delete('static_analysis', array('assignment'=>$id));

		//First remove all previous problems
		$this->db->delete('problems', array('assignment'=>$id));

		//Now add new problems:
		$names = $this->input->post('name');
		$scores = $this->input->post('score');
		$c_tl = $this->input->post('c_time_limit');
		$py_tl = $this->input->post('python_time_limit');
		$java_tl = $this->input->post('java_time_limit');
		$ml = $this->input->post('memory_limit');
		$ft = $this->input->post('languages');
		$dc = $this->input->post('diff_cmd');
		$da = $this->input->post('diff_arg');
		$uo = $this->input->post('is_upload_only');
		$sa = $this->input->post('static_analysis');
		$weight = $this->input->post('weight');
		if ($uo === NULL)
			$uo = array();
		if ($sa === NULL)
			$sa = array();
		for ($i=1; $i<=$this->input->post('number_of_problems'); $i++)
		{
			$items = explode(',', $ft[$i-1]);
			$ft[$i-1] = '';
			foreach ($items as $item){
				$item = trim($item);
				$item2 = strtolower($item);
				$item = ucfirst($item2);
				if ($item2 === 'python2')
					$item = 'Python 2';
				elseif ($item2 === 'python3')
					$item = 'Python 3';
				elseif ($item2 === 'pdf')
					$item = 'PDF';
				$item2 = strtolower($item);
				if ( ! in_array($item2, array('c','c++','python 2','python 3','java','zip','pdf')))
					continue;
				// If the problem is not Upload-Only, its language should be one of {C,C++,Python 2, Python 3,Java}
				if ( ! in_array($i, $uo) && ! in_array($item2, array('c','c++','python 2','python 3','java')) )
					continue;
				$ft[$i-1] .= $item.",";
			}
			$ft[$i-1] = substr($ft[$i-1],0,strlen($ft[$i-1])-1); // remove last ','
			$problem = array(
				'assignment' => $id,
				'id' => $i,
				'name' => $names[$i-1],
				'score' => $scores[$i-1],
				'is_upload_only' => in_array($i,$uo)?1:0,
				'c_time_limit' => $c_tl[$i-1],
				'python_time_limit' => $py_tl[$i-1],
				'java_time_limit' => $java_tl[$i-1],
				'memory_limit' => $ml[$i-1],
				'allowed_languages' => $ft[$i-1],
				'diff_cmd' => $dc[$i-1],
				'diff_arg' => $da[$i-1],
				'weight'   => $weight[$i-1],
				'static_analysis' => in_array($i,$sa)?1:0
			);
			$this->db->insert('problems', $problem);
			
			/* **** Adding static analysis to "static_analysis" table **** */
			if (in_array($i,$sa)){
				//Now add new static analysis:
				$static_analysis = array(
					'assignment' => $id,
					'problem' => $i,
					'public_methods' => $this->input->post('public_methods'),
					'public_methods_max' => $this->input->post('public_methods_max'),
					'auxiliary_classes' => $this->input->post('auxiliary_classes'),
					'auxiliary_classes_max' => $this->input->post('auxiliary_classes_max'),
					'unnecessary_attributes' => $this->input->post('unnecessary_attributes'),
					'unnecessary_attributes_max' => $this->input->post('unnecessary_attributes_max'),
					'lower_camel_case' => $this->input->post('lower_camel_case'),
					'lower_camel_case_max' => $this->input->post('lower_camel_case_max'),
					'code_quality' => $this->input->post('code_quality'),
					'code_quality_max' => $this->input->post('code_quality_max'),
					'duplicated_code' => $this->input->post('duplicated_code'),
					'static_analysis_weight' => $this->input->post('static_analysis_weight')
				);
				$this->db->insert('static_analysis', $static_analysis);
			};
		}

		
		if ($edit)
		{
			// We must update scoreboard of the assignment
			$this->load->model('scoreboard_model');
			$this->scoreboard_model->update_scoreboard($id);
		}

		// Complete Database Transaction
		$this->db->trans_complete();

		return $this->db->trans_status();
	}



	// ------------------------------------------------------------------------



	/**
	 * Delete An Assignment
	 *
	 * @param $assignment_id
	 */
	public function delete_assignment($assignment_id)
	{
		$this->db->trans_start();

		// Phase 1: Delete this assignment and its submissions from database
		$this->db->delete('assignments', array('id'=>$assignment_id));
		$this->db->delete('problems', array('assignment'=>$assignment_id));
		$this->db->delete('submissions', array('assignment'=>$assignment_id));
		$this->db->delete('static_analysis', array('assignment'=>$assignment_id));

		$this->db->trans_complete();

		if ($this->db->trans_status())
		{
			// Phase 2: Delete assignment's folder (all test cases and submitted codes)
			$cmd = 'rm -rf '.rtrim($this->settings_model->get_setting('assignments_root'), '/').'/assignment_'.$assignment_id;
			shell_exec($cmd);
		}
	}



	// ------------------------------------------------------------------------



	/**
	 * All Assignments
	 *
	 * Returns a list of all assignments and their information
	 *
	 * @return mixed
	 */
	public function all_assignments()
	{
		$result = $this->db->order_by('id')->get('assignments')->result_array();
		$assignments = array();
		foreach ($result as $item)
		{
			$assignments[$item['id']] = $item;
		}
		return $assignments;
	}

	// ------------------------------------------------------------------------



	/**
	 * All Assignments of user classes
	 *
	 * Returns a list of all assignments and their information
	 *
	 * @return mixed
	 */
	public function all_assignments_classes($classes_id)
	{
		$this->db->select('assignment_id');
		if($classes_id == []){
			$this->db->where('class_id', NULL);
		}else{
			$this->db->where_in('class_id', $classes_id)->or_where('class_id', NULL);
		}
		$query_assignments_classes = $this->db->get('assignments_classes');
		$assignments_id = array();
		foreach ($query_assignments_classes->result() as $row) {
			array_push($assignments_id, $row->assignment_id);
		}
		if ($assignments_id == []) {
			return;
		}
		$this->db->where_in('id', $assignments_id);
		$result = $this->db->order_by('id')->get('assignments')->result_array();
		$assignments = array();
		foreach ($result as $item)
		{
			$assignments[$item['id']] = $item;
		}
		return $assignments;
	}




	// ------------------------------------------------------------------------



	/**
	 * All Assignments
	 *
	 * Returns a list of all assignments id
	 *
	 * @return mixed
	 */
	public function get_all_assignments_id()
	{
		$query = $this->db->select('id')->get('assignments');
		if ($query->num_rows() <= 0)
			return FALSE;
		return $query->result();
	}



	// ------------------------------------------------------------------------



	/**
	 * New Assignment ID
	 *
	 * Finds the smallest integer that can be uses as id for a new assignment
	 *
	 * @return int
	 */
	public function new_assignment_id()
	{
		$max = ($this->db->select_max('id', 'max_id')->get('assignments')->row()->max_id) + 1;

		$assignments_root = rtrim($this->settings_model->get_setting('assignments_root'), '/');
		while (file_exists($assignments_root.'/assignment_'.$max)){
			$max++;
		}

		return $max;
	}



	// ------------------------------------------------------------------------



	/**
	 * All Problems of an Assignment
	 *
	 * Returns an array containing all problems of given assignment
	 *
	 * @param $assignment_id
	 * @return mixed
	 */
	public function all_problems($assignment_id)
	{
		$result = $this->db->order_by('id')->get_where('problems', array('assignment'=>$assignment_id))->result_array();
		$problems = array();
		foreach ($result as $row)
			$problems[$row['id']] = $row;
		return $problems;
	}



	// ------------------------------------------------------------------------



	/**
	 * Static Analysis of an Assignment
	 *
	 * Returns an array containing static anaysis of given assignment
	 *
	 * @param $assignment_id
	 * @return mixed
	 */
	public function static_analysis_info($assignment_id)
	{
		$static_analysis = $this->db->order_by('assignment')->get_where('static_analysis', array('assignment'=>$assignment_id))->result_array();
		return $static_analysis[0];
	}



	// ------------------------------------------------------------------------



	/**
	 * Problem Info
	 *
	 * Returns database row for given problem (from given assignment)
	 *
	 * @param $assignment_id
	 * @param $problem_id
	 * @return mixed
	 */
	public function problem_info($assignment_id, $problem_id)
	{
		return $this->db->get_where('problems', array('assignment'=>$assignment_id, 'id'=>$problem_id))->row_array();
	}



	// ------------------------------------------------------------------------



	/**
	 * Assignment Info
	 *
	 * Returns database row for given assignment
	 *
	 * @param $assignment_id
	 * @return array
	 */
	public function assignment_info($assignment_id)
	{
		$query = $this->db->get_where('assignments', array('id'=>$assignment_id));
		if ($query->num_rows() != 1){
			return array(
				'id' => 0,
				'name' => 'Not Selected',
				'finish_time' => 0,
				'extra_time' => 0,
				'problems' => 0
			);
		}
		$info = $query->row_array();
		$query_classes = $this->db->get_where('assignments_classes', array('assignment_id'=>$assignment_id));
		$classes = array();
		foreach($query_classes->result_array() as $class){
			if($class['class_id'] == NULL ){
				$info['classes_selected'] = NULL;
				return $info;
			}
			array_push($classes, $class['class_id']);
		}
		$info['classes_selected'] = $classes;
		return $info;
	}



	// ------------------------------------------------------------------------



	/**
	 * Is Participant
	 *
	 * Returns TRUE if $username is one of the $participants
	 * Examples for participants: "ALL" or "user1, user2,user3"
	 *
	 * @param $participants
	 * @param $username
	 * @return bool
	 */
	public function is_participant($participants, $username)
	{
		$participants = explode(',', $participants);
		foreach ($participants as &$participant){
			$participant = trim($participant);
		}
		if(in_array('ALL', $participants))
			return TRUE;
		if(in_array($username, $participants))
			return TRUE;
		return FALSE;
	}



	// ------------------------------------------------------------------------



	/**
	 * Increase Total Submits
	 *
	 * Increases number of total submits for given assignment by one
	 *
	 * @param $assignment_id
	 * @return mixed
	 */
	public function increase_total_submits($assignment_id)
	{
		// Get total submits
		$total = $this->db->select('total_submits')->get_where('assignments', array('id'=>$assignment_id))->row()->total_submits;
		// Save total+1 in DB
		$this->db->where('id', $assignment_id)->update('assignments', array('total_submits'=>($total+1)));

		// Return new total
		return ($total+1);
	}



	// ------------------------------------------------------------------------



	/**
	 * Set Moss Time
	 *
	 * Updates "Moss Update Time" for given assignment
	 *
	 * @param $assignment_id
	 */
	public function set_moss_time($assignment_id)
	{
		$now = shj_now_str();
		$this->db->where('id', $assignment_id)->update('assignments', array('moss_update'=>$now));
	}



	// ------------------------------------------------------------------------



	/**
	 * Get Moss Time
	 *
	 * Returns "Moss Update Time" for given assignment
	 *
	 * @param $assignment_id
	 * @return string
	 */
	public function get_moss_time($assignment_id)
	{
		$query = $this->db->select('moss_update')->get_where('assignments', array('id'=>$assignment_id));
		if($query->num_rows() != 1) return 'Never';
		return $query->row()->moss_update;
	}



	// ------------------------------------------------------------------------


	/**
	 * Save Problem Description
	 *
	 * Saves (Adds/Updates) problem description (html or markdown)
	 *
	 * @param $assignment_id
	 * @param $problem_id
	 * @param $text
	 * @param $type
	 */
	public function save_problem_description($assignment_id, $problem_id, $text, $type)
	{
		$assignments_root = rtrim($this->settings_model->get_setting('assignments_root'), '/');

		if ($type === 'html')
		{
			// Remove the markdown code
			unlink("$assignments_root/assignment_{$assignment_id}/p{$problem_id}/desc.md");
			// Save the html code
			file_put_contents("$assignments_root/assignment_{$assignment_id}/p{$problem_id}/desc.html", $text);
		}
		elseif ($type === 'md')
		{
			// We parse markdown using Parsedown library
			$this->load->library('parsedown');
			// Save the markdown code
			file_put_contents("$assignments_root/assignment_{$assignment_id}/p{$problem_id}/desc.md", $text);
			// Convert markdown to html and save the html
			file_put_contents("$assignments_root/assignment_{$assignment_id}/p{$problem_id}/desc.html", $this->parsedown->parse($text));
		}

	}


	// ------------------------------------------------------------------------


	/**
		 * Export assignments and problems as a CSV file 
		 *
		 *
		 * @return $mixed
		 */
		public function exportAssignments_csv()
		{
			$this->load->dbutil();
			$queryAssignments = $this->db->query("SELECT * FROM shj_assignments");
			$queryProblems = $this->db->query("SELECT * FROM shj_problems");
			return array(
				'backupAssignments' => $this->dbutil->csv_from_result($queryAssignments),
				'backupProblems' => $this->dbutil->csv_from_result($queryProblems)
			);
		}

	// ------------------------------------------------------------------------


	/**
		 * Delete rows of table assignments_classes of specific assignment_id 
		 *
		 * This function is called when editing an assignment 
		 * 
		 */

		public function delete_assignments_classes_rows($assignment_id)
		{
			$this->db->delete('assignments_classes', array('assignment_id'=>$assignment_id));
		}


	// ------------------------------------------------------------------------


	/**
	 * Update Coefficients
	 *
	 * Each time we edit an assignment (Update start time, finish time, extra time, or
	 * coefficients rule), we should update coefficients of all submissions of that assignment
	 *
	 * This function is called from add_assignment($id, TRUE)
	 *
	 * @param $assignment_id
	 * @param $extra_time
	 * @param $finish_time
	 * @param $new_late_rule
	 */
	private function _update_coefficients($assignment_id, $extra_time, $finish_time, $new_late_rule)
	{
		$submissions = $this->db->get_where('submissions', array('assignment'=>$assignment_id))->result_array();

		$finish_time = strtotime($finish_time);

		foreach ($submissions as $i => $item) {
			$delay = strtotime($item['time'])-$finish_time;
			ob_start();
			if ( eval($new_late_rule) === FALSE )
				$coefficient = "error";
			if (!isset($coefficient))
				$coefficient = "error";
			ob_end_clean();
			$submissions[$i]['coefficient'] = $coefficient;
		}
		// For better performance, we update each 1000 rows in one SQL query
		$size = count($submissions);
		for ($i=0; $i<=($size-1)/1000; $i++) {
			if ($this->db->dbdriver === 'postgre')
				$query = 'UPDATE '.$this->db->dbprefix('submissions')." AS t SET coefficient = c.coeff FROM (values \n";
			else
				$query = 'UPDATE '.$this->db->dbprefix('submissions')." SET coefficient = CASE\n";

			for ($j=1000*$i; $j<1000*($i+1) && $j<$size; $j++){
				$item = $submissions[$j];
				if ($this->db->dbdriver === 'postgre'){
					$query.="($assignment_id, {$item['problem']}, '{$item['username']}', {$item['submit_id']}, '{$item['coefficient']}')";
					if ($j+1<1000*($i+1) && $j+1<$size )
						$query.=",\n";
				}
				else
					$query.="WHEN assignment='$assignment_id' AND problem='{$item['problem']}' AND username='{$item['username']}' AND submit_id='{$item['submit_id']}' THEN {$item['coefficient']}\n";
			}

			if ($this->db->dbdriver === 'postgre')
				$query.=") AS c(assignment, problem, username, submit_id, coeff)\n"
				."WHERE t.assignment=c.assignment AND t.problem=c.problem AND t.username=c.username AND t.submit_id=c.submit_id;";
			else
				$query.="ELSE coefficient \n END \n WHERE assignment='$assignment_id';";
			$this->db->query($query);
		}
	}


}