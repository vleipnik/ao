import { Resolved, fromPromise, of } from 'hyper-async'
import { z } from 'zod'

import { findProcessSchema, loadMessageMetaSchema, locateProcessSchema } from '../dal.js'
import { findRawTag, trimSlash } from '../utils.js'

/**
 * The result that is produced from this step
 * and added to ctx.
 *
 * This is used to parse the output to ensure the correct shape
 * is always added to context
 */
const ctxSchema = z.object({
  processId: z.any().refine((val) => !!val, {
    message: 'processId must be attached to context'
  }),
  timestamp: z.number().refine((val) => !!val, {
    message: 'timestamp must be attached to context'
  }),
  /**
   * nonce can be 0, so we can't just use falsey here.
   *
   * So instead cast to null and compare
   */
  nonce: z.number().refine((val) => val != null, {
    message: 'nonce must be attached to context'
  })
}).passthrough()

function maybeCachedWith ({ locateProcess, findProcess }) {
  locateProcess = fromPromise(locateProcessSchema.implement(locateProcess))
  findProcess = fromPromise(findProcessSchema.implement(findProcess))

  /**
   * Attempt to use the cached process to locate the scheduler
   * via it's Scheduler-Location record, which prevents potentially
   * calling a gateway to load a process
   */
  return (processId) => findProcess({ processId })
    .bichain(
      () => Resolved(undefined),
      (process) => of(process)
        .map((process) => findRawTag('Scheduler', process.tags))
        .map((tag) => tag ? tag.value : undefined)
    )
    .chain((maybeSchedulerHint) => locateProcess({ processId, schedulerHint: maybeSchedulerHint }))
}

/**
 * @typedef Args
 * @property {string} messageTxId - the transaction id of the message
 *
 * @typedef Result
 * @property {string} processId - the id of the process that the message is for
 * @property {string} timestamp - the timestamp of the message
 *
 * @callback LoadMessageMeta
 * @param {Args} args
 * @returns {Async<Result>}
 *
 * @param {Env} env
 * @returns {LoadMessageMeta}
 */
export function loadMessageMetaWith (env) {
  const logger = env.logger.child('loadMessageMeta')
  env = { ...env, logger }

  const loadMessageMeta = fromPromise(loadMessageMetaSchema.implement(env.loadMessageMeta))

  const maybeCached = maybeCachedWith(env)

  return (ctx) => {
    return maybeCached(ctx.processId)
      .chain(({ url }) => loadMessageMeta({
        suUrl: trimSlash(url),
        processId: ctx.processId,
        messageTxId: ctx.messageTxId
      }))
      .map(ctxSchema.parse)
      // .map(logger.tap('Loaded message process and timestamp and appended to ctx %j'))
  }
}
