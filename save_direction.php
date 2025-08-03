<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST');
header('Access-Control-Allow-Headers: Content-Type');

require_once 'db_config.php';

try {
    $pdo = getDbConnection();
    
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        $direction = $_POST['direction'] ?? '';
        
        if (in_array($direction, ['forward', 'backward'])) {
            $pdo->exec("DELETE FROM directions");
            
            $stmt = $pdo->prepare("INSERT INTO directions (direction) VALUES (?)");
            $stmt->execute([$direction]);
            
            echo json_encode([
                'success' => true,
                'message' => 'Direction saved successfully',
                'direction' => $direction
            ]);
        } else {
            echo json_encode([
                'success' => false,
                'message' => 'Invalid direction. Must be "forward" or "backward"'
            ]);
        }
    } else {
        echo json_encode([
            'success' => false,
            'message' => 'Only POST method allowed'
        ]);
    }
    
} catch (Exception $e) {
    echo json_encode([
        'success' => false,
        'message' => 'Database error: ' . $e->getMessage()
    ]);
}
?>