chai = require('chai')
assert = chai.assert
should = chai.should()
expect = chai.expect

util   = require('./utils.coffee')
target = './shell'

describe 'command parser', ->

  runTest = (cfg, done) ->
    util.run(target, cfg.cli).then (result) ->
      expect(result.stderr).to.have.length(0)
      util.checkCmdTable(result.stdout, cfg.tab, cfg.flags)
      done()
    # Error check
    .catch (error) -> done(error)
    .done()

  it 'should correctly recognize a basic command', (done) ->
    runTest
      cli: 'command'
      tab: [ ['command'] ]
      flags: ['default', 'default', 'default', 'NO', 'NO']
      done

  it 'should correctly recognize a basic command with one argument', (done) ->
    runTest
      cli: 'command arg0'
      tab: [ ['command', 'arg0'] ]
      flags: ['default', 'default', 'default', 'NO', 'NO']
      done

  it 'should correctly recognize a basic command with multiple arguments', (done) ->
    runTest
      cli: 'command arg0 arg1 arg2 arg3 arg4'
      tab: [ ['command', 'arg0', 'arg1', 'arg2', 'arg3', 'arg4', 'arg5' ] ]
      flags: ['default', 'default', 'default', 'NO', 'NO']
