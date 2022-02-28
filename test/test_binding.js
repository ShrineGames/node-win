const Windows = require("../dist/binding.js");
const assert = require("assert");

function testBasic() {
    assert.strictEqual(true, true);
    // const result = Windows.Stream
    // assert.strictEqual(result, "world", "Unexpected value returned");
}

// assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");