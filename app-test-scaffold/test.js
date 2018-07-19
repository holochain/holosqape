var test = require('tape');

test('timing test', function (t) {
    t.plan(5);
    t.equal(3, 3);
    t.equal(Container.installedApps().length, 3);
    t.equal(Container.installedApps()[0], "A3F21RqpuAse0U8UqodHW8yi/Sh7elz5FJ4sAcevChg=");
    t.equal(Container.installedApps()[1], "ZJ4w6xur5xqwWSZlcaQf7mfr+tBbf3owiC1XKWkDgLo=");
    t.equal(Container.installedApps()[2], "uTS2nMUDg0xs+o9QQtZz0QoIFFdfv8teQse82qMsXCg=");
});
