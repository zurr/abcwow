<?php
ini_set('max_execution_time',0);

class DBC2SQL {
    var $fh;
	var $out_fh;
    var $header;
    var $dbcName;
    var $layout;
	var $out_type;

    function convert($filename,$out_t = 'file'){
		$this->out_type = $out_t;
		if ($this->out_type == 'file'){
			define('endl',"\n");
			define('tabl',"\t");
			$this->out_fh = fopen($filename.'.sql', 'w');
			if (!$this->out_fh) print('Unable to create '.$filename.'.sql');
		} else {
				define('endl','<br />');
				define('tabl',' ');
		}
		
        if (!is_file($filename.'.dbc')) print('Unable to find '.$filename);
        $this->fh = fopen($filename.'.dbc', 'r');
        if (!$this->fh) print('Unable to open '.$filename);
        if (!$this->checkDBC()) print('Invalid file '.$filename);
        $this->dbcName = $filename;
        $xml = simplexml_load_string(file_get_contents('dbclayout.xml'));
        
        $this->layout = (array)$xml->{$this->dbcName};
 
        $this->output("-- ".$this->dbcName.endl);
        $this->output("-- Records: ".$this->header['nRecords'].endl);
        $this->output("-- Fields: ".$this->header['nFields'].endl);
        $this->output("-- String table: ".$this->header['stringBSize']." bytes".endl);
		$this->output("-- -----------------------------".endl.endl);
        ob_flush();
        $this->createTable();

        $this->getData();

		if ($this->out_type == 'file'){
			fclose($this->out_fh);
		}
		
		print $filename." DONE...";
    }


    function checkDBC(){
        if (fread($this->fh, 4) !== 'WDBC') return false;
        list(, $this->header['nRecords']) = unpack('I*', fread($this->fh, 4));
        list(, $this->header['nFields']) = unpack('I*', fread($this->fh, 4));
        list(, $this->header['recordSize']) = unpack('I*', fread($this->fh, 4));
        list(, $this->header['stringBSize']) = unpack('I*', fread($this->fh, 4));
        if ($this->header['recordSize'] != ( $this->header['nFields'] * 4) ) return false;
        return true;
    }

    function createTable(){
        $query = 'DROP TABLE IF EXISTS dbc_'.strtolower($this->dbcName).";";
        $this->output($query.endl);
        $query = 'CREATE TABLE `dbc_'.strtolower($this->dbcName).'` ('.endl;
        foreach($this->layout as $data)
            foreach($data as $field){
                $query .= tabl.'`'.$field['name'].'` ';
                switch($field['type']){
                    case 'int':
                        $query .= 'bigint(20) NOT NULL DEFAULT \'0\',';
                        break;
                    case 'string':
                        $query .= 'text,';
                        break;
                    case 'float':
                        $query .= 'float NOT NULL DEFAULT \'0\',';
                };
                $query .= endl;
            };
        $query .= tabl.'PRIMARY KEY (`entry`)'.endl;
        $query .= ") COMMENT='Export of ".$this->dbcName.".dbc';";
        $this->output($query.endl.endl);
		$this->output('INSERT INTO `dbc_'.strtolower($this->dbcName).'` VALUES '.endl);
    }

    function getData(){
        fseek($this->fh, 20);
		$count_rows = 0;
        for ($i = 1; $i <= $this->header['nRecords']; $i++){
			$count_rows++;
            foreach($this->layout as $data){
				
				$this->output( "(");
				$c = 0;
                foreach($data as $field) {
					$c++;
					$this->output( $this->getField($i, $field['id'], $field['type']));
					if ($c != count($data)) $this->output( ",");
					}
				$this->output(")");

				if ($count_rows >= 20){
						$this->output(";".endl);
						$this->output('INSERT INTO `dbc_'.strtolower($this->dbcName).'` VALUES '.endl);
						$count_rows = 0;
				} else {
						if ($i != $this->header['nRecords']) $this->output(",".endl);
							else $this->output(";".endl);
						}
			}
			
			
        };
    }

    function getField($row, $field, $type){
        fseek($this->fh, 20 + ( $this->header['recordSize'] * ($row - 1) ) + ( ($field - 1) * 4 ), SEEK_SET);
        switch($type){
            case 'int':
                list(, $r) = unpack('I*', fread($this->fh, 4));
                return (int)$r;
                break;
            case 'string':
                list(, $r) = unpack('I*', fread($this->fh, 4));
				$tmp = str_replace("\n", "<br />", (string)$this->getString($r));
				$tmp = str_replace("\r", " ", $tmp);
                return "\"".addslashes($tmp)."\"";
                break;
            case 'float':
                list(, $r) = unpack('f*', fread($this->fh, 4));
                return (float)round($r, 4);
                break;
            default:
                break;
        };
        return null;
    }

    function getString($offset){

        if ($this->header['stringBSize'] < 1) return false;
        fseek($this->fh, (($this->header['recordSize'] * $this->header['nRecords']) + 20 + ($offset - 1)), SEEK_SET);
        $str = '';
        if (ord(fread($this->fh, 1)) !== 0) return $str;
        while ($c = (int)ord(fread($this->fh, 1))){
            if ($c !== '\0')
                $str .= chr($c);
            else
                break;
        };

        return (string)$str;
    }
	
	
	function output($data){
		if ($this->out_type == 'file') fwrite($this->out_fh, $data);
			else print $data;
	}

};

/*##################################################################################333*/

	$class = new DBC2SQL;
	$class->convert('Spell',"file");

?>