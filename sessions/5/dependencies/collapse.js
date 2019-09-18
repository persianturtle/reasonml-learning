/** @flow */
/**
 * Collapse
 *
 * A utility component to handle animated collapse/expand functionality without an opinionated UI layer
 * Use the provided render props to hook up any UI you pass in
 *
 *
 * @author    Rogin Farrer <rfarrer@wayfair.com>
 * @copyright 2018 Wayfair LLC - All rights reserved
 */
import React, { PureComponent, type Node, type Ref } from 'react'
import { scheduleAnimationCallback } from 'hb-schedule-animation-callback'
import Uid from 'hb-uid'

// Helper function for render props. Sets a function to be called, plus any additional functions passed in
const callAll = (...fns) => (...args: Array<*>) =>
  fns.forEach(fn => fn && fn(...args))

type CollapseWithIdPropType = {
  children: ({
    isOpen: boolean,
    getTogglerProps: (*) => { onClick: ?() => void },
    getCollapsibleProps: (*) => {},
    contentRef: Ref<*>
  }) => Node,
  // If true, the collapsed content is visible. If true or false, the visibility of the content is controlled
  isOpen: ?boolean,
  // If true, the collapsed content is visible on mount
  defaultOpen: boolean,
  // The height of the content when collapsed
  collapsedHeight: number | 'auto',
  // The duration of the collapse/expand transition, in MS
  duration: number,
  // The timing function used in the transition
  timingFunction: string,
  // The duration of the collapse transition.
  // If not defined, falls back to normal `duration`
  closeDuration?: number,
  // The timing function used in the collapse transition
  // If not defined, falls back to normal `timingFunction`
  closeTimingFunction?: string,
  // The duration of the expand transition.
  // If not defined, falls back to normal `duration`
  openDuration?: number,
  // The timing function used in the expand transition
  // If not defined, falls back to normal `timingFunction`
  openTimingFunction?: string,
  // An optional max-height for the collapse content
  maxHeight?: ?number
}

type UidPropType = {
  uid: number
}

type CollapsePropType = CollapseWithIdPropType & UidPropType

type CollapseStateType = {
  isOpen: boolean
}

const DEFAULT_DURATION = 300
const DEFAULT_TIMING = 'cubic-bezier(0.09, 1.03, 0.57, 0.97)'

// If defaultOpen is true, we need to use the natural height of the element
// we get from `contentRef` to avoid an unwanted transition on mount
const getInitialHeight = ({
  isOpen,
  defaultOpen,
  maxHeight,
  collapsedHeight
}) => {
  if ((typeof isOpen === 'boolean' && isOpen) || defaultOpen) {
    return maxHeight ? `${maxHeight}px` : 'auto'
  } else {
    return `${collapsedHeight}px`
  }
}

class Collapse extends PureComponent<CollapsePropType, CollapseStateType> {
  static defaultProps = {
    isOpen: null,
    defaultOpen: false,
    collapsedHeight: 0,
    duration: DEFAULT_DURATION,
    timingFunction: DEFAULT_TIMING,
    maxHeight: null
  }

  state = {
    isOpen:
      typeof this.props.isOpen === 'boolean'
        ? this.props.isOpen
        : this.props.defaultOpen
  }

  componentDidMount() {
    if (this.collapsible) {
      const collapsible: HTMLElement = this.collapsible
      const isOpen = this.getIsOpen()
      if (isOpen && collapsible.clientHeight !== 0) {
        collapsible.style.height = this.props.maxHeight
          ? `${this.props.maxHeight}px`
          : 'auto'
      }
      // set `display: none` only if the content is not visible
      else if (!isOpen && this.props.collapsedHeight === 0) {
        collapsible.style.display = 'none'
      }
    }
  }

  componentDidUpdate(
    prevProps: CollapsePropType,
    prevState: CollapseStateType
  ) {
    const isCurrentlyOpen = this.getIsOpen()

    if (this.getIsOpen(prevState, prevProps) !== isCurrentlyOpen) {
      this.isTransitioning = true
      clearTimeout(this.transitionTimeoutId)
      this.transitionTimeoutId = setTimeout(() => {
        this.isTransitioning = false
      }, parseInt(this.getTransitionDurationStyle(isCurrentlyOpen), 10))

      if (isCurrentlyOpen) {
        // reset the isClosing flag to ensure we don't re-close when we should be open
        this.isClosing = false
        this.setOpen()
      } else {
        // set the isClosing flag here to indicate we have started to close
        this.isClosing = true

        if (this.collapsible) {
          const collapsible: HTMLElement = this.collapsible
          // reset height from 'auto' to pixel height for smooth collapse animation
          collapsible.style.height = this.getHeight(true)
        }

        // start collapse animation once the browser is ready
        scheduleAnimationCallback(() => {
          scheduleAnimationCallback(() => {
            // ensure we still want to close before closing
            // this is here so we don't attempt to finish closing
            // if we should re-open
            if (this.isClosing) {
              this.setClosed()
            }
          })
        })
      }
    }
  }

  collapsible: ?HTMLElement
  content: ?HTMLElement
  isClosing: boolean = false
  isTransitioning: boolean = false
  transitionTimeoutId: TimeoutID

  /**
   * Returns the state of the isOpen prop.
   * If it is controlled, return the prop value.
   * If is isn't, use internal state
   */
  getIsOpen(
    state: $Shape<CollapseStateType> = this.state,
    props: $Shape<CollapsePropType> = this.props
  ): boolean {
    return typeof props.isOpen === 'boolean' ? props.isOpen : state.isOpen
  }

  // If defaultOpen is true, we need to use the natural height of the element
  // we get from `contentRef` to avoid an unwanted transition on mount
  getHeight(isOpen) {
    if (!isOpen) {
      return `${this.props.collapsedHeight}px`
    }
    if (this.props.maxHeight) {
      return `${this.props.maxHeight}px`
    }
    if (this.content) {
      return `${this.content.clientHeight}px`
    }
    return 'auto'
  }

  setOpen = () => {
    if (this.collapsible) {
      const collapsible: HTMLElement = this.collapsible
      collapsible.style.willChange = 'height'
      collapsible.style.display = ''
    }

    /**
     * Allow the browser to remove the `display: none` property while also
     * giving it sufficent time to apply ahead-of-time optimizations for
     * `willChange`
     */
    scheduleAnimationCallback(() => {
      if (!this.isClosing && this.collapsible) {
        const collapsible: HTMLElement = this.collapsible
        collapsible.style.height = this.getHeight(true)
      }
    })
  }

  setClosed = () => {
    if (this.collapsible) {
      const collapsible: HTMLElement = this.collapsible
      collapsible.style.willChange = 'height'
    }

    /**
     * Allow the browser to give sufficent time to apply ahead-of-time
     * optimizations for `willChange`
     */
    scheduleAnimationCallback(() => {
      if (this.isClosing && this.collapsible) {
        const collapsible: HTMLElement = this.collapsible
        collapsible.style.height = this.getHeight(false)
      }
    })
  }

  toggleIsOpen = () => this.setState(({ isOpen }) => ({ isOpen: !isOpen }))

  /**
   * At the end of the transition open, make the height of the collapsible 'auto'.
   * This will prevent overflow and height issues if the content of the collapsible
   * changes while the panel is open
   *
   * At the end of the transition close, make the display of the collapsible 'none'
   */
  handleCollapsibleTransitionEnd = () => {
    // if not yet done transitioning, try again in the future
    if (this.isTransitioning) {
      scheduleAnimationCallback(this.handleCollapsibleTransitionEnd)
      return
    }

    if (this.collapsible) {
      const collapsible: HTMLElement = this.collapsible
      if (this.getIsOpen()) {
        collapsible.style.willChange = ''

        if (collapsible.clientHeight !== 0) {
          collapsible.style.height = this.props.maxHeight
            ? `${this.props.maxHeight}px`
            : 'auto'
        }
      } else if (this.props.collapsedHeight === 0) {
        collapsible.style.display = 'none'
      }
    }
  }

  getTogglerProps = (props: { onClick: ?() => void } = { onClick() {} }) => {
    return {
      id: `CollapseToggle-${this.props.uid}`,
      'aria-controls': `CollapsePanel-${this.props.uid}`,
      'aria-expanded': this.getIsOpen(),
      tabIndex: 0,
      type: 'button',
      ...props,
      onClick: callAll(props.onClick, this.toggleIsOpen)
    }
  }

  /**
   * Returns the value for the transition-duration css property
   * If user supplies a custom open or close duration, use that.
   * Otherwise fallback to the `duration` prop
   */
  getTransitionDurationStyle = (isOpen: boolean) => {
    const { closeDuration, openDuration, duration } = this.props
    const actualDuration = isOpen
      ? openDuration || duration
      : closeDuration || duration
    return `${actualDuration}ms`
  }

  /**
   * Returns the value for the transition-timing-function css property
   * If user supplies a custom open or close timing function, use that.
   * Otherwise fallback to the `timingFunction` prop
   */
  getTransitionTimingFunctionStyle = (isOpen: boolean) => {
    const {
      closeTimingFunction,
      openTimingFunction,
      timingFunction
    } = this.props
    return isOpen
      ? openTimingFunction || timingFunction
      : closeTimingFunction || timingFunction
  }

  getCollapsibleProps = (
    collapsibleProps: { refKey: string, ref: any } = {
      refKey: 'ref',
      ref: null
    }
  ) => {
    const { refKey = 'ref', ...rest } = collapsibleProps
    const isOpen = this.getIsOpen()
    return {
      id: `CollapsePanel-${this.props.uid}`,
      'aria-hidden': !isOpen,
      onTransitionEnd: this.handleCollapsibleTransitionEnd,
      'data-enzyme-id': 'Collapse-Collapsible',
      ...rest,
      [refKey]: callAll(this.assignCollapsibleRef, collapsibleProps[refKey]),
      style: {
        height: this.collapsible
          ? this.collapsible.style.height
          : getInitialHeight(this.props),
        overflow: 'hidden',
        transitionProperty: 'height',
        transitionDuration: this.getTransitionDurationStyle(isOpen),
        transitionTimingFunction: this.getTransitionTimingFunctionStyle(isOpen)
      }
    }
  }

  assignCollapsibleRef = (node: ?HTMLElement) => {
    this.collapsible = node
  }

  assignContentRef = (node: ?HTMLElement) => (this.content = node)

  render() {
    return this.props.children({
      isOpen: Boolean(this.getIsOpen()),
      getTogglerProps: this.getTogglerProps,
      getCollapsibleProps: this.getCollapsibleProps,
      contentRef: this.assignContentRef
    })
  }
}

// $FlowIgnore - Flow doesn't know about createContext yet
const Context = React.createContext()
export const { Consumer } = Context
const { Provider: BareProvider } = Context

type ProviderProps = {
  children: Node
}

export default function CollapseWithId(props: CollapseWithIdPropType) {
  return <Uid>{id => <Collapse {...props} uid={id} />}</Uid>
}

export const Provider = ({ children, ...rest }: ProviderProps): Node => (
  <CollapseWithId {...rest}>
    {props => <BareProvider value={props}>{children}</BareProvider>}
  </CollapseWithId>
)

// @ignore-prop-docs
CollapseWithId.defaultProps = Collapse.defaultProps
