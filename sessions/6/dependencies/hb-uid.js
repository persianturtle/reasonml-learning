/**
 * HB Unique ID
 *
 * Utility for creating unique IDs
 *
 * @flow
 * @author    Rogin Farrer <rfarrer@wayfair.com>
 * @copyright 2019 Wayfair LLC - All rights reserved
 */
import { Component } from 'react'

export default class Uid extends Component {
  static counter = 0
  state = { uid: 0 }
  componentDidMount() {
    this.setState({ uid: Uid.counter++ })
  }
  render() {
    return this.props.children(this.state.uid)
  }
}
