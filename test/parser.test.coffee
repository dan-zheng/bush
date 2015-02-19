chai = require('chai')
assert = chai.assert
should = chai.should()
expect = chai.expect

util   = require('./utils.coffee')
target = './shell'

describe 'Parser', ->

  runTest = (cfg, done) ->
    util.run(target, cfg.cli).then (result) ->
      expect(result.stderr).to.have.length(0)
      util.checkCmdTable(result.stdout, cfg.tab, cfg.flags)
      done()
    # Error check
    .catch (error) -> done(error)
    .done()

  describe 'Basic Command', ->

    it 'should correctly parse a basic command', (done) ->
      runTest
        cli: 'command'
        tab: [ ['command'] ]
        done

    it 'should correctly parse a basic command with one argument', (done) ->
      runTest
        cli: 'command arg0'
        tab: [ ['command', 'arg0'] ]
        done

    it 'should correctly parse a basic command with multiple arguments', (done) ->
      runTest
        cli: 'command arg0 arg1 arg2 arg3 arg4'
        tab: [ ['command', 'arg0', 'arg1', 'arg2', 'arg3', 'arg4'] ]
        done

  describe 'IO Redirect', ->

    it 'should correctly parse > file', (done) ->
      runTest
        cli: 'command > outfile'
        tab: [ ['command'] ]
        flags:
          outfile: 'outfile'
        done

    it 'should correctly parse >> file', (done) ->
      runTest
        cli: 'command >> outfile'
        tab: [ ['command'] ]
        flags:
          outfile: 'outfile'
          truncate: 'NO'
        done

    it 'should correctly parse >& file', (done) ->
      runTest
        cli: 'command >& outfile'
        tab: [ ['command'] ]
        flags:
          outfile: 'outfile'
          errfile: 'outfile'
        done

    it 'should correctly parse >>& file', (done) ->
      runTest
        cli: 'command >>& outfile'
        tab: [ ['command'] ]
        flags:
          outfile: 'outfile'
          errfile: 'outfile'
          truncate: 'NO'
        done

    it 'should correctly parse < file', (done) ->
      runTest
        cli: 'command < infile'
        tab: [ ['command'] ]
        flags:
          infile: 'infile'
        done

    it 'should correctly parse redirects regardless whether there are whitespaces', (done) ->
      runTest
        cli: 'command<infile'
        tab: [ ['command'] ]
        flags:
          infile: 'infile'
        done

    it 'should correctly handle `command > file1 > file2`', (done) ->
      runTest
        cli: 'command > file1 > file2'
        tab: [ ['command'] ]
        flags:
          outfile: 'file2'
        done

    it 'should correctly handle `command >> file1 > file2`', (done) ->
      runTest
        cli: 'command >> file1 > file2'
        tab: [ ['command'] ]
        flags:
          outfile: 'file2'
          truncate: 'NO'
        done

    it 'should correctly handle `command >& file1 > file2`', (done) ->
      runTest
        cli: 'command >& file1 > file2'
        tab: [ ['command'] ]
        flags:
          outfile: 'file2'
          errfile: 'file2'
        done

    it 'should correctly handle `command >>& file1 > file2`', (done) ->
      runTest
        cli: 'command >>& file1 > file2'
        tab: [ ['command'] ]
        flags:
          outfile: 'file2'
          errfile: 'file2'
          truncate: 'NO'
        done
