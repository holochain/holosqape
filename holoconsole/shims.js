function setTimeout(callback, milliseconds) {
    return console.setTimeout(callback, milliseconds);
}

function clearTimeout(timeout) {
    console.clearTimeout(timeout);
}


console["warn"] = console.log
console["trace"] = console.log

"shims installed."
