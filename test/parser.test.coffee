chai = require('chai')
assert = chai.assert
should = chai.should()
expect = chai.expect

util = require('./utils.coffee')

util.run './shell', 'aaa bbb > outfile &'
  .then (result) ->
    console.log util.stripAnsi(result.stdout)
  .catch (error) ->
    console.log error
