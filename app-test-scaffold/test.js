var test = require('tape');

test('timing test', function (t) {
    t.plan(4);
    t.equal(3, 3);
    t.equal(Container.installedApps().length, 2);
    t.equal(Container.installedApps()[0], "A3F21RqpuAse0U8UqodHW8yi/Sh7elz5FJ4sAcevChg=");
    t.equal(Container.installedApps()[1], "uTS2nMUDg0xs+o9QQtZz0QoIFFdfv8teQse82qMsXCg=");
});
