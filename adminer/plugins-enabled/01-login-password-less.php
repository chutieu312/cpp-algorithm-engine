<?php
class AdminerLoginPasswordLess {
    function login($login, $password) {
        return true;
    }
}

return new AdminerLoginPasswordLess;
