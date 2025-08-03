<?php
$db_config = [
    'host' => 'localhost', 
    'username' => 'root',
    'password' => '',
    'database' => 'servo_control'
];

function getDbConnection() {
    global $db_config;
    
    try {
        $dsn = "mysql:host={$db_config['host']};dbname={$db_config['database']};charset=utf8mb4";
        $pdo = new PDO($dsn, $db_config['username'], $db_config['password']);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        $pdo->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);
        return $pdo;
    } catch (PDOException $e) {
        throw new Exception("Database connection failed: " . $e->getMessage());
    }
}
?>