'use strict'
// let Agent = require("can").Agent;
// let agent = new Agent();
// console.log(agent);  //输出 world
const bindings = require('bindings')
const binding = bindings('can')  //调用StreamWritable::Init
// const { Agent } = binding

// const agent = new Agent();

module.exports = binding

// const { StreamWritable } = binding
//
// const stream = require('stream')
// const Writable = stream.Writable
//
// class WritableWrapper extends Writable {
//   constructor(opts = {}) {
//     super(opts)
//     this._writable = new StreamWritable()  //调用StreamWritable::New  && StreamWritable::StreamWritable
//     // this.another_writable = new StreamWritable()  //又调用一遍StreamWritable::New  && StreamWritable::StreamWritable
//   }
//
//   _write(chunk, encoding, cb) {
//     this._writable.write(chunk)
//     cb()
//   }
// }
//
// module.exports = WritableWrapper
