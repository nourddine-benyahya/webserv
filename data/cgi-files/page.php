#! /usr/bin/php

<?php
    echo "Hello World!";
    //get data from QUERY_STRING
    echo "<h1> HELLO </h1>";
    $data = getenv('QUERY_STRING');
    //parse data
    parse_str($data, $data);
    //print data
    print_r($data);

?>