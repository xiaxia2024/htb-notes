
```
cat reset_password.php
<?php
header('Content-Type: application/json');

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $username = $_POST['username'];
    $db = new SQLite3('private_34eee5d2/db.sqlite');
    $stmt = $db->prepare('SELECT * FROM users WHERE username = :username');
    $stmt->bindValue(':username', $username, SQLITE3_TEXT);
    $result = $stmt->execute();
    $user = $result->fetchArray(SQLITE3_ASSOC);

    if ($user) {
        $newPassword = bin2hex(random_bytes(4)); // Generate 8-character password
        $hashedPassword = sha1($newPassword);
        $updateStmt = $db->prepare('UPDATE users SET password_hash = :password_hash WHERE username = :username');
        $updateStmt->bindValue(':password_hash', $hashedPassword, SQLITE3_TEXT);
        $updateStmt->bindValue(':username', $username, SQLITE3_TEXT);
        $updateStmt->execute();

        echo json_encode([
            'username' => $username,
            'new_password' => $newPassword,
            'timestamp' => date('Y-m-d H:i:s')
        ]);
    } else {
        echo json_encode(['error' => 'User not found']);
    }
} else {
    echo json_encode(['error' => 'Invalid request']);
}
?>
```
