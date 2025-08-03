<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET');

require_once 'db_config.php';

try {
    $pdo = getDbConnection();
    
    $stmt = $pdo->query("SELECT direction, timestamp FROM directions ORDER BY timestamp DESC LIMIT 1");
    $result = $stmt->fetch();
    
    if ($result) {
        $angle = ($result['direction'] === 'forward') ? 180 : 0;
        
        echo json_encode([
            'success' => true,
            'direction' => $result['direction'],
            'angle' => $angle,
            'timestamp' => $result['timestamp']
        ]);
    } else {
        echo json_encode([
            'success' => true,
            'direction' => 'none',
            'angle' => 90,
            'timestamp' => null
        ]);
    }
    
} catch (Exception $e) {
    echo json_encode([
        'success' => false,
        'message' => 'Database error: ' . $e->getMessage()
    ]);
}
?>